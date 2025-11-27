<?php
/**
 *  RestClient Class
 *
 *
 */
class RestClient {
  public $api_url;
  public $token;
  public $request;
  public $response;
  public $data;
  public $operation;

  public function __construct(array $options=[]) {
    $this->api_url = variable_get('megaphone_api_url', 'https://cms.megaphone.fm/api');
    $this->token = variable_get('megaphone_token', false);

    $default_options = [
        'headers' => ['Authorization' => "Bearer $this->token"],
      ];
    $merged_options = array_merge($default_options, $options);
    $this->options = $merged_options;
  }

  public function setAPIUrl ( string $url ) {
    $this->api_url = $url;
  }

  public function setToken (string $token) {
    $this->token = $token;
    if ($this->options['headers']['Authorization']) $this->options['headers']['Authorization'] = "Bearer this->token";
  }

  public function setOperation ( string $op = '' ) {
    $this->operation = $op;
  }

  // Request methods

  public function get(string $url, $parameters=[], array $headers=[])  {
    return $this->execute($url, 'GET', $parameters, $headers);
  }

  public function post(string $url, $parameters=[], array $headers=[])  {
      return $this->execute($url, 'POST', $parameters, $headers);
  }

  public function put(string $url, $parameters=[], array $headers=[])  {
      return $this->execute($url, 'PUT', $parameters, $headers);
  }

  public function patch(string $url, $parameters=[], array $headers=[])  {
      return $this->execute($url, 'PATCH', $parameters, $headers);
  }

  public function delete(string $url, $parameters=[], array $headers=[])  {
      return $this->execute($url, 'DELETE', $parameters, $headers);
  }

  public function head(string $url, $parameters=[], array $headers=[])  {
      return $this->execute($url, 'HEAD', $parameters, $headers);
  }

  public function execute (string $url, string $method='GET', $parameters=[], array $headers=[])  {
    // Store Operation
    $trace = debug_backtrace(2, 3);
    $this->setOperation($trace[2]['function']);

    // Store Request
    $this->request = [
      'url' => $url,
      'headers' => array_merge($this->options['headers'], $headers),
      'method' => $method,
      'parameters' => $parameters,
    ];

    //  Build URL
    $url = sprintf(
      "%s/%s",
      rtrim((string) $this->api_url, '/'),
      ltrim((string) $url, '/')
    );

    // Build Options
    $options = [
      'headers' => $headers,
      'method' => $method,
    ];
    if (!empty($parameters)) {
      switch ($method) {
        case "GET":
          $url += "?". drupal_http_build_query($parameters);
          break;
        default:
          $options['data'] = json_encode($parameters);
          break;
      }
    }
    $options = array_merge_recursive($this->options, $options);

    //  Send Request and store response.
    $this->response = drupal_http_request($url, $options);

    //  Check for error and throw
    $this->checkForError();

    //  Return data
    if (!empty($this->response->data)) {
      $this->data = json_decode($this->response->data);
      return $this->data;
    }
  }

  /**
   * @method public checkForError()
   *
   * @return void
   */
  public function checkForError(string $title = "API Error") {
    // common vars
    $op = $this->operation;
    $code = $this->response->code;
    $data = isset($this->response->data) ? json_decode($this->response->data) : null;
    $data_string = isset($data) ? json_encode($data, JSON_PRETTY_PRINT) : null;
    $request = $this->request['method'] . " " . $this->request['url'];

    // message determined by error/success
    if (isset($this->response->error)) {
      $error = $this->response->error;
      $subject = "$op => $code $error";
      $message = "$error\n\n$data_string\n\n$request\n";
      // communicate to admin user
      drupal_set_message("$title: $subject\n$message", 'error');
      // log in drupal
      watchdog($title, "$subject\n$message");
    } else {
      $subject = "$op => $code";
      $message = "$data_string\n\n$request\n";
      $error = false;
    }

    return [
      $op,
      $subject,
      $message,
      $error
    ];
  }

}
