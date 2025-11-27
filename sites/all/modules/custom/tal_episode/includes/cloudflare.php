<?php
/**
 *  CFRestClient Class
 *
 *
 */
class CFRestClient extends RestClient {
    private $account_id;
    private $workflow_id;
    private $secret;
    private $worker_url;
    private $use_worker;

    public function __construct(array $options=[]) {
        parent::__construct($options);
        $this->api_url = 'https://api.cloudflare.com/client/v4/';
        $this->token = variable_get('cloudflare_token', false);
        $this->account_id = variable_get('cloudflare_account_id', false);
        $this->workflow_id = variable_get('cloudflare_workflow_id', false);
        $this->secret = variable_get('cloudflare_secret', false);
        $this->worker_url = variable_get('cloudflare_worker_url', false);
        $this->use_worker = false;

        $default_options = [
            'headers' => ['Authorization' => "Bearer $this->token"],
        ];
        $merged_options = array_merge($default_options, $options);
        $this->options = $merged_options;
    }

    /**
     * Set use_worker property to true or false.  When true, api_url will be substituted with worker_url.
     *
     * @param boolean $yesorno
     * @return void
     */
    public function useWorker( bool $yesorno) {
        $this->use_worker = $yesorno;
    }

    /**
     * Create workflow instance.
     *
     * @see https://developers.cloudflare.com/api/resources/workflows/subresources/instances/methods/create/
     * @param array $params
     * @return void
     */
    public function createWorkflowInstance( array $params ) {
        //  exit without queue item id
        if (!array_key_exists('item_id',$params)) throw new Error("item_id required", 1);
        
        //  Set endpoint.
        if (
            $this->use_worker &&
            isset($this->worker_url)
        ) {
            $this->setAPIUrl($this->worker_url);
            $endpoint = "";
        } else {
            $endpoint = "accounts/$this->account_id/workflows/$this->workflow_id/instances";
        }

        //  Assemble params.
        $secret = hash_hmac('sha256', $params['item_id'], $this->secret);
        $params['secret'] = $secret;
        $default_params = [
        'url' => url("tal/cron/queue", array('absolute' => true)),
        'queue_id' => 'audio',
        ];
        $params = [ 'params' => array_merge($default_params, $params) ];

        //  Compose headers.
        $headers = ["Content-Type" => "application/json"];
        return $this->post($endpoint, $params, $headers);
    }

    /**
     * Get workflow instance status.
     *
     * @see https://developers.cloudflare.com/api/resources/workflows/subresources/instances/methods/get/
     * @param string $instance_id
     * @return void
     */
    public function getWorkflowInstanceStatus( string $instance_id ) {
        $endpoint = "accounts/$this->account_id/workflows/$this->workflow_id/instances/$instance_id";
        return $this->get($endpoint, $params);
    }
}