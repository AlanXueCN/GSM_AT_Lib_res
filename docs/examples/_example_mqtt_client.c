#include "mqtt.h"
#include "gsm/gsm.h"
#include "apps/gsm_mqtt_client.h"

mqtt_client_t* mqtt_client;
 
/*
 * \brief           Connection information for MQTT CONNECT packet
 */
const mqtt_client_info_t
mqtt_client_info = {
    .id = "test_client_id",                     /* The only required field for connection! */
    
    // .keep_alive = 10,
    // .user = "test_username",
    // .pass = "test_password",
};

static void mqtt_evt(mqtt_client_t* client, mqtt_evt_t* evt);
static void example_do_connect(mqtt_client_t* client);

/*
 * \brief           MQTT client thread
 * \param[in]       arg: User argument
 */
void
mqtt_thread(void const* arg) {
    /*
     * Create a new client with 256 bytes of RAW TX data
     * and 128 bytes of RAW incoming data
     */
    mqtt_client = mqtt_client_new(256, 128);
    mqtt_client_connect(mqtt_client, "test.mosquitto.org", 1883, mqtt_evt, &mqtt_client_info);
    
    while (1) {
        osDelay(5000);
    }
}

/*
 * \brief           MQTT event callback function
 * \param[in]       client: MQTT client where event occurred
 * \param[in]       evt: Event type and data
 */
static void
mqtt_evt(mqtt_client_t* client, mqtt_evt_t* evt) {
    switch (gsm_evt_get_type(evt)) {
        /*
         * Connect event
         * Called if user successfully connected to MQTT server
         * or even if connection failed for some reason
         */
        case MQTT_EVT_CONNECT: {                /* MQTT connect event occurred */
            mqtt_conn_status_t status = evt->evt.connect.status;
            
            if (status == MQTT_CONN_STATUS_ACCEPTED) {
                /*
                 * Once we are accepted on server, 
                 * it is time to subscribe to differen topics
                 * We will subscrive to "mqtt_gsm_example_topic" topic,
                 * and will also set the same name as subscribe argument for callback later
                 */
                mqtt_client_subscribe(client, "mqtt_gsm_example_topic", MQTT_QOS_AT_MOST_ONCE, "mqtt_gsm_example_topic");
            } else {
                printf("MQTT server connection was not successful: %d\r\n", (int)status);
                /* Maybe close connection at this point and try again? */
            }
            break;
        }
        
        /*
         * Subscribe event just happened.
         * Here it is time to check if it was successful or failed attempt
         */
        case MQTT_EVT_SUBSCRIBE: {
            const char* arg = evt->evt.sub_unsub_scribed.arg;   /* Get user argument */
            gsmr_t res = evt->evt.sub_unsub_scribed.res;    /* Get result of subscribe event */
            
            if (res == gsmOK) {
                printf("Successfully subscribed to %s topic\r\n", arg);
                if (!strcmp(arg, "mqtt_gsm_example_topic")) {   /* Check topic name we were subscribed */
                    /* Subscribed to "example_topic" topic */
                    
                    /*
                     * Now publish an even on example topic
                     * and set QoS to minimal value which does not guarantee message delivery to received
                     */
                    mqtt_client_publish(client, "mqtt_gsm_example_topic", "my_data", 7, MQTT_QOS_AT_MOST_ONCE, 0, (void *)1);
                }
            }
            
            break;
        }
        
        /*
         * Message published event occurred
         */
        case MQTT_EVT_PUBLISHED: {              /* MQTT publish was successful */
            uint32_t val = (uint32_t)evt->evt.published.arg;    /* Get user argument, which is in fact our custom number */
            
            printf("Publish was successful, user argument on message was: %d\r\n", (int)val);
            
            break;
        }
        
        /*
         * A new message was published to us
         * and now it is time to read the data
         */
        case MQTT_EVT_PUBLISH_RECV: {
            const char* topic = (const char *)evt->evt.publish_recv.topic;
            uint16_t topic_len = evt->evt.publish_recv.topic_len;
            const uint8_t* payload = evt->evt.publish_recv.payload;
            uint16_t payload_len = evt->evt.publish_recv.payload_len;
            
            /* Check topic for publish command */
            if (!memcmp("mqtt_gsm_example_topic", topic, topic_len)) {
                printf("MQTT publish received on topic: %.*s\r\n", topic_len, topic);
            }
            
            GSM_UNUSED(payload);
            GSM_UNUSED(payload_len);
            break;
        }
        
        /*
         * Client is fully disconnected from MQTT server
         */
        case MQTT_EVT_DISCONNECT: {
            printf("MQTT client disconnected!\r\n");
            example_do_connect(client);         /* Connect to server */
            break;
        }
        
        default: 
            break;
    }
}

/* Make a connection to MQTT server in non-blocking mode */
static void
example_do_connect(mqtt_client_t* client) {
    /*
     * Start a simple connection to open source
     * MQTT server on mosquitto.org
     */
    mqtt_client_connect(client, "test.mosquitto.org", 1883, mqtt_evt, &mqtt_client_info);
}