<?php
/**
 *  MegaRestClient Class
 *
 *
 */
class MegaRestClient extends RestClient {
  private $organization_id;
  private $network_id;
  private $podcast_id;
  private $TZ;
  private $episode_id;
  private $nid;

  public function __construct(array $options=[]) {
    parent::__construct($options);
    $this->network_id = variable_get('megaphone_network_id', false);
    $this->organization_id = variable_get('megaphone_organization_id', false);
    $this->podcast_id = variable_get('megaphone_podcast_id', false);
    $this->api_url = variable_get('megaphone_api_url', 'https://cms.megaphone.fm/api');
    $this->token = variable_get('megaphone_token', false);
    $this->TZ = new DateTimeZone("America/New_York");

    $this->nid = false;

    $default_options = [
      'headers' => ['Authorization' => "Token token=$this->token"],
    ];
    $merged_options = array_merge($default_options, $options);
    $this->options = $merged_options;
  }

  public function setNetworkId (string $Id) {
    $this->network_id = $Id;
  }

  public function setOrganizationId (string $Id) {
    $this->organization_id = $Id;
  }

  public function setPodcastId (string $Id) {
    $this->podcast_id = $Id;
  }

  public function setEpisodeId ( string $Id = '' ) {
    $this->episode_id = $Id;
  }

  public function setToken (string $token) {
    $this->token = $token;
    if ($this->options['headers']['Authorization']) $this->options['headers']['Authorization'] = "Token token=$this->token";
  }

  public function setNid ( $nid = 0 ) {
    $this->nid = $nid;
  }

  //  Megaphone methods.

  /**
   * @method public podcasts()
   *
   * @return object
   */
  public function podcasts() {
    $endpoint = "networks/$this->network_id/podcasts";
    return $this->get($endpoint);
  }

  /**
   * @method public podcast()
   *
   * @param string $pid
   * @return object
   */
  public function podcast( $pid = null ) {
    $pid = (isset($pid)) ?: $this->podcast_id;
    $endpoint = "networks/$this->network_id/podcasts/$pid";
    return $this->get($endpoint);
  }

  /**
   * @method public getEpisodes()
   *
   * @param array $filter - format ['episode_property_name' => 'value to test for equality']. all conditions must be met.
   * @return array - megaphone episode objects keyed by episode id
   */
  public function getEpisodes($filter = [], $key = 'id') {
    $episodes = &drupal_static(__FUNCTION__);
    if (!isset($episodes)) {
      $episodes = $this->episode();
      $perPage = $this->response->headers['x-per-page'];
      $page = $this->response->headers['x-page'];
      $total = $this->response->headers['x-total'];
      while (count($episodes) < $total ) {
        $page++;
        $endpoint = "networks/$this->network_id/podcasts/$this->podcast_id/episodes?page=$page";
        $more = $this->get($endpoint);
        $episodes = array_merge($episodes,$more);
      }
    }

    if (!empty($filter)) {
      $filtered = [];
      foreach ($episodes as $ep) {
        $return = false;
        foreach ($filter as $key => $value) {
          if (isset($ep->$key) && ($ep->$key == $value)) {
            $return = true;
          } else {
            $return = false;
            break;
          }
        }
        if ($return) $filtered[$ep->id] = $ep;
      }
      return $filtered;
    } else {
      $keyed = [];
      foreach ($episodes as $ep) {
        $keyed[$ep->{$key}] = $ep;
      }
      return $keyed;
    }
  }

  /**
   * @method public episode()
   *
   * @param string $episode_id
   * @return object
   */
  public function episode( string $episode_id = '') {
    $endpoint = "networks/$this->network_id/podcasts/$this->podcast_id/episodes";
    if (empty($episode_id)) return $this->get($endpoint);
    else {
      $this->setEpisodeId($episode_id);
      return $this->get("$endpoint/$episode_id");
    }
  }

  /**
   * @method public episodeCreate()
   *
   * @param array $attributes
   * @return object
   */
  public function episodeCreate( $attributes = [] ) {
    $endpoint = "networks/$this->network_id/podcasts/$this->podcast_id/episodes";
    $pubDate = $this->getStubAirDate();
    $default_attributes = [
      'title' => "Episode Stub - $pubDate",
      'author' => 'This American Life',
      'pubdate' => $pubDate,
      'expectedAdhash'=> '011112',
      'draft' => true,
    ];
    $attributes = array_merge($default_attributes, $attributes);
    $headers = ["Content-Type" => "application/json"];
    $result = $this->post($endpoint, $attributes, $headers);
    $episode_id = $this->data->id ?: '';
    $this->setEpisodeId($episode_id);
    return $result;
  }

  /**
   * @method public episodeInitiateUpload()
   *
   * @param string $mid
   * @param string $audioUrl
   * @param boolean|int $nid - Schedule node ID from which to upload podcast audio file
   * @return object
   */
  public function episodeInitiateUpload( string $episode_id, string $audioUrl, $originalUrl = false ) {
    $endpoint = "networks/$this->network_id/podcasts/$this->podcast_id/episodes/$episode_id";
    $attributes = [
      'backgroundAudioFileUrl' => $audioUrl,
      'originalUrl' => $originalUrl ?: $audioUrl
    ];
    $this->setEpisodeId($episode_id);
    return $this->put($endpoint, $attributes);
  }

  /**
   * @method public episodeUpdate()
   *
   * @param string $episode_id
   * @param array $attributes
   * @return object
   */
  public function episodeUpdate ( string $episode_id, $attributes = [] ) {
    $endpoint = "networks/$this->network_id/podcasts/$this->podcast_id/episodes/$episode_id";
    $this->setEpisodeId($episode_id);
    return $this->put($endpoint, $attributes);
  }

  /**
   * Update episode Insertion Points
   *
   * @param string $episode_id
   * @param array $midroll
   * @return object
   */
  public function episodeUpdateMidroll ( string $episode_id, array $midroll ) {
    $endpoint = "networks/$this->network_id/podcasts/$this->podcast_id/episodes/$episode_id";
    $midtimes = array_map([$this, "timecodeToSeconds"], $midroll);
    $attributes = [
      'insertionPoints' => [ 0.01, ...$midtimes]
    ];
    $this->setEpisodeId($episode_id);
    return $this->put($endpoint, $attributes);
  }

  /**
   * @method public episodeDelete()
   *
   * @param string $episode_id
   * @return object
   */
  public function episodeDelete ( string $episode_id ) {
    $endpoint = "networks/$this->network_id/podcasts/$this->podcast_id/episodes/$episode_id";
    $this->setEpisodeId($episode_id);
    return $this->delete($endpoint);
  }

  /**
   * Log API transactions.
   *
   * @param integer $nid
   * @param string|null $message - custom message appears after response
   * @param string|null $op - override operation
   * @return void
   */
  public function log( int $nid = 0, string $message = null, string $op = null) {
    if (is_null($nid)) $nid = 0;
    $message = (strlen($message) > 0) ? " -- $message" : $message;
    $message = trim($message);
    $message = substr($message, 0, 60);
    db_insert('tal_megaphone_log')
    ->fields([
      'nid' => $this->nid ?: $nid,
      'timestamp' => time(),
      'mid' => $this->episode_id,
      'operation' => $op ?: $this->operation,
      'request' => $this->request["url"],
      'response' => $this->response->code . ":" . $message,
    ])
    ->execute();
  }

  /**
   * @method public checkForError()
   *
   * @return void
   */
  public function checkForError( string $title = 'Megaphone API') {
    [$op, $subject, $message, $error] = parent::checkForError($title);
    if ($error) {

      // notify those who want to know
      tal_episode_megaphone_email_notification("error_$op","$title Error: $subject",$message);

      // Throw an exception in order to halt any related node operations
      switch ($op) {
        case 'episodeCreate':
        case 'episodeUpdate':
        case 'episodeDelete':
          // log
          $this->log(null, "$message");
          // throw error
          throw new ErrorException("$title: $subject\n$message", E_WARNING);
          break;

        default:
          break;
      }
    } else {
      // notify those who want to know
      tal_episode_megaphone_email_notification("success_$op","$title Success: $subject",$message);
    }
  }

  /*    Helper Methods   */

  /**
   * @method public getStubAirDate()
   *
   * @param boolean $seconds
   * @return string|int
   */
  public function getStubAirDate( $seconds = false ) {
    $oneYearFromNow = date_create("friday +1 year", $this->TZ);
    $return = $oneYearFromNow->format("Y-m-d") . "T00:00:00";
    if ($seconds) $return = strtotime($return);

    return $return;
  }

  /**
   * Convert timecode to Megaphone appropriate seconds value.
   *
   * @param string $timecode
   * @return float
   */
  public function timecodeToSeconds ($timecode) {
    // get base seconds
    $t = explode(':', $timecode);
    $seconds = (float) array_pop($t);

    // loop through the rest
    $x = 60;
    while (count($t) > 0) {
      $more = (float) array_pop($t);
      $seconds = $seconds + ($x * $more);
      $x = $x*$x;
    }
    return $seconds;
  }

}
