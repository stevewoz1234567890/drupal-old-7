<?php

/**
 * Endpoint for Cloudflare Workflow which processes queue items one at a time.
 *
 *      We use this strategy to avoid being dependent on cron so that queue items
 *      begin to be processed as soon as they are created.
 *
 * @param integer $id
 * @param string $qname
 * @return HTTP
 */
function tal_episode_queue_cron( string $qname, int $id ) {
    $result = tal_episode_queue_process_item($qname, $id);
    drupal_json_output($result);
    drupal_exit();
}

/**
 * Queue runner.
 *
 * 	Checks out a queue item and send to worker for processing.  Returns an array of data to be consumed by Cloudflare workflow.
 *
 * @param string $name
 * @param int $id
 * @param int $time
 * @return array
 */
function tal_episode_queue_process_item( string $name, int $id, $time = 30) {
	if ($q = new TalEpisodeQueue("tal_episode_queue_$name")) {
		$result = [ 'item' => false ];
		if ($item = $q->claimItemById($id, $time)) {
			$result['item'] = $item;
			$workerFunc = "tal_episode_queue_worker_$name";
			try {
				//  Call the queue worker
				$workerResult = $workerFunc( unserialize($item->data) );
				//  Save returned output.
				if (is_array($workerResult)) $result = array_merge($result, $workerResult);
				//  On success, delete item.
				if ($result['finished'] && !$result['error'] ) $q->deleteItem($item);
			} catch (\Throwable $th) {
				$result['error'] = $th->getCode().":".$th->getMessage();
			}
		}
		return $result;
	}
}

/**
 * Audio Update Queue Worker.
 *
 *	- Polls Megaphone for audio processing completion.
 *	- Throws an exception and sends an email if audio processing finishes but is not sucessful.
 *
 * @param array $data - ['nid' => schedule node id, 'field' => audio field name]
 * @return void
 */
function tal_episode_queue_worker_audio( $data ) {
	['nid' => $nid, 'field' => $field] = $data;
	$finished = false;
	$message = "";
	$error = false;
	if (
		($schedule = node_load($nid)) &&
		($wrapper = entity_metadata_wrapper('node', $schedule)) &&
		($mid = $wrapper->field_megaphone_episode_id->value()) &&
		($file = $wrapper->{"field_audio_$field"}->value())
	) {
		$api = new MegaRestClient();
		$api->setNid($nid);
		$mega_ep = $api->episode($mid);
		$parts = pathinfo($file['uri']);
		$filename = $parts['filename'] ."." . $parts['extension'];
		$local_ep = $wrapper->field_episode->value();
		$local_wrapper = entity_metadata_wrapper('node', $local_ep);
		$ep_num = $local_wrapper->field_episode_number->value();
		$ep_title = $local_ep->title;
		$ep_air_date = $wrapper->field_radio_air_date->value();
		$mega_link = "https://cms.megaphone.fm/organizations/".variable_get('megaphone_organization_id')."/podcasts/$mega_ep->podcastId/episodes/$mega_ep->id";
		$body = "Episode: $ep_title (#$ep_num)\n" .
			"Air Date: $ep_air_date\n" .
			"Megaphone Link: $mega_link\n";

		//  First: Confirm upload success
		if (
			($mega_ep->originalFilename == $filename) &&
			(!$mega_ep->audioFileProcessing) &&
			($mega_ep->audioFileStatus == 'success')
		) {
			//  log & send email notification
			$message = "$filename successfully processed";
			$api->log($nid, $message, "Audio File Processing");
			tal_episode_megaphone_email_notification(
				"success_audiofileprocessing",
				"Audio file successfully processed",
				$body . $message
			);
			//  Then: Update Midroll.
			if (['episode' => $episode] = tal_episode_megaphone_update_midroll($schedule)) {
				$finished = true;
				tal_episode_megaphone_email_notification(
					"success_episodeupdatemidroll",
					"Midroll value successfully updated",
					$body
				);
			}
		} else {
			switch ($episode->audioFileStatus) {
				case "processing":
					break;
				case "no_audio":
					$message = "No audio.";
					break;
				case "error":
					$message = "$filename failed to process.";
					$finished = true;
					break;
			}
			//  log and send email notification
			if (strlen($message) > 0) {
				$api->log($nid, $message, "Audio File Processing");
				tal_episode_megaphone_email_notification(
					"error_audiofileprocessing",
					"Audio Processing: " . $message,
					$body
				);
			}
		}
	}
	return [
		'finished' => $finished,
		'message' => $message,
		'error' => $error,
	];
}
