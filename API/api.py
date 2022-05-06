from influxdb import InfluxDBClient
from flask import Flask, request, render_template
import paho.mqtt.client as mqtt

MQTT_SERVER = "localhost"
MQTT_SERVERPORT = 1883
MQTT_USERNAME = "Dragomin"
MQTT_KEY = "Nerd"
# define MQTT_TOPIC_COUNT "/count"
# define MQTT_TOPIC_VOLT  "/voltage"

app = Flask(__name__)

mqttclient = mqtt.Client(MQTT_USERNAME)
mqttclient.username_pw_set(MQTT_USERNAME, MQTT_KEY)
influxclient = InfluxDBClient(host='localhost', port=8086)


@app.route('/')
def index():
    return 'Index page'


@app.route('/get_db_data/<db>/<table>')
def show_user(db=None, table=None):
    query_result = ""
    # returns all the data contained in the <db> database
    try:
        influxclient.switch_database(db)
        # TODO sanitize parameters here
        query_result = influxclient.query('SELECT * FROM ' + table).raw
    except:
        return "Error: something went wrong, go figure why\n"
    return query_result

# Receives a post request with Json data containing a duration field


@app.route('/activate_pump', methods=['POST'])
def activate_pump():
    content = request.json
    duration = content['duration']
    # Activates the pump for <duration> seconds
    mqttclient.connect(MQTT_SERVER, port=MQTT_SERVERPORT)
    mqttclient.publish("Dragomin/pump/manual_activation", duration)
    return content["duration"]

    # TODO authenticate API user with oauth

# In case you want to post to a new topic, you need to add the redirection
# within the telegraf service, from the MQTT broker to the InfluxDB.
# Do that by editing the config in /etc/telegraf/telegraf.conf (search for Dragomin)
# And then restart the service with systemctl reload telegraf
