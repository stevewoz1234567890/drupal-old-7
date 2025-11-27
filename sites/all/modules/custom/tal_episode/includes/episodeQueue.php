<?php
/**
 *    TalEpisodeQueue Class
 *
 *
 *
 */
class TalEpisodeQueue extends SystemQueue {
  /**
   * @method public createItem()
   *
   *    Do not create if item with data already exists.
   *    Create Cloudflare workflow instance on create.
   *
   * @param mixed $data
   * @return boolean
   */
  public function createItem($data) {
    // Check if already exists.
    $result = db_query('SELECT COUNT(item_id) FROM {queue} WHERE name = :name AND data = :data', array(
      ':name' => $this->name,
      ':data' => serialize($data),
      ))->fetchField();
    $exists = (int) $result;

    if ($exists === 0) {
      // Create item.
      $query = db_insert('queue')->fields(array(
        'name' => $this->name,
        'data' => serialize($data),
        'created' => time(),
      ));
      $qiid = $query->execute();

      // Send to Cloudflare.
      $api = new CFRestClient();
      if (
        ($workflow = $api->createWorkflowInstance(['item_id' => $qiid])) &&
        $workflow->success
      ) {
        $wfid = $workflow->result->id;
        $message = "Cloudflare workflow instance id $wfid instantiated for tal queue item id $qiid";
        $level = WATCHDOG_INFO;
      } else {
        $message = "Cloudflare workflow instantiation failed for tal queue item id $qiid";
        $level = WATCHDOG_ERROR;
        $item = (object)['item_id' => $qiid];
        $this->deleteItem($item);
      }

      //  Log and return result.
      watchdog('TalEpisodeQueue->createItem', $message, $level);
      if ($level == WATCHDOG_ERROR) {
        return false;
      } else {
        return $qiid;
      }

    } else {
      return false;
    }
  }

  /**
   * @method public claimItemById
   *
   *    Enable claim item by specified ID for running queue with Cloudflare workflow.
   *
   * @param integer $id
   * @param integer $lease_time
   * @return void
   */
  public function claimItemById( int $id, int $lease_time = 30) {
    $item = db_query_range('SELECT data, item_id FROM {queue} q WHERE name = :name AND item_id = :id ORDER BY created, item_id ASC', 0, 1, array(
      ':name' => $this->name,
      ':id' => (string) $id,
    ))
    ->fetchObject();

    if ($item) {
        return $item;
    } else {
        // No items currently available to claim.
        return false;
    }
  }
}
