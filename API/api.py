from influxdb import InfluxDBClient
from flask import Flask, request, render_template
import paho.mqtt.client as mqtt

MQTT_SERVER = "localhost"
MQTT_SERVERPORT = 1883
MQTT_USERNAME = "Dragomin"
# define MQTT_KEY         "Nerd"
# define MQTT_TOPIC_COUNT "/count"
# define MQTT_TOPIC_VOLT  "/voltage"

app = Flask(__name__)

mqttclient = mqtt.Client(MQTT_USERNAME)
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
        query_result = influxclient.query('SELECT * FROM ' + table).raw
    except:
        return "Error: something went wrong, go figure why\n"
    return query_result


@app.route('/activate_pump', methods=['POST'])
def activate_pump():
    duration = request.form['duration']
    username = request.form['username']
    password = request.form['password']
    # Activates the pump for <duration> seconds
    mqttclient.connect(MQTT_SERVER, port=MQTT_SERVERPORT)
    mqttclient.publish("pump/activation", duration)

    # TODO authenticate API user with oauth
    # TODO setup broker > DB redirection
    # TODO connect to local influx database for querying
    # TODO input sanitation in the requests
