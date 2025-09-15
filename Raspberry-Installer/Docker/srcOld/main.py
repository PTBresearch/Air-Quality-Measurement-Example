# Start Befehle
# uvicorn main:app --reload --host 0.0.0.0 --port 30000
# uvicorn main:app --reload --host 0.0.0.0 --port 30000 --ssl-keyfile meinserver.key --ssl-certfile meinserver.cert

from fastapi import FastAPI
from starlette.responses import JSONResponse
from starlette.requests import Request
from multiprocessing import Process
from threading import Thread
from fiware_loader import FiwareLoader

import ast
import json

import requests, uvicorn, time, yaml, ssl
import logging

app = FastAPI()
ssl_keyfile = ""
ssl_certfile = ""
httpProxy = ""
httpsProxy = ""

with open('config.yml', 'r') as file:
    config = yaml.safe_load(file)

with open('indoorEnvironmentObserved.jsonld') as file:
    json_data = file.read()
    indoor_env_json = json.loads(json_data)


if 'loglevel' in config:
    loglevel = config['loglevel']
else:
    loglevel = ""

loglevel = loglevel.lower()

if 'logformat' in config:
    logformat = config['logformat']
else:
    logformat = "%(asctime)s - %(name)s - %(levelname)s - %(message)s"

log_config = uvicorn.config.LOGGING_CONFIG
log_config["formatters"]["access"]["fmt"] = logformat
log_config["formatters"]["default"]["fmt"] = logformat

if loglevel == 'debug':
    logging.basicConfig(level=logging.DEBUG, format=logformat)
elif loglevel == 'info':
    logging.basicConfig(level=logging.INFO, format=logformat)
elif loglevel == 'warning':
    logging.basicConfig(level=logging.WARNING, format=logformat)
elif loglevel == 'critical':
    logging.basicConfig(level=logging.CRITICAL, format=logformat)
elif loglevel == 'error':
    logging.basicConfig(level=logging.ERROR, format=logformat)
else:
    logging.basicConfig(level=logging.INFO, format=logformat)
    if loglevel == '':
        logging.warning("loglevel nicht gesetzt")
    else:
        logging.warning("loglevel falsch geschrieben")
    loglevel = "info"

luftapi_logger = logging.getLogger("luftapi")

logging.info("------------------------------------------------------------------------------")
logging.info("Starte Luftqualiäts API")

if loglevel == 'debug':
    logging.info("LOGLEVEL wurde auf " + loglevel.upper() + " gesetzt")

# API
if 'api' not in config:
    luftapi_logger.error("API not configured")
    exit(1)

if 'http'not in config['api']:
    luftapi_logger.error("API http not configured")
    exit(1)

if 'port' not in config['api']['http']:
    luftapi_logger.error("API http port not configured")
    exit(1)
httpPort = config['api']['http']['port']

if 'https' not in config['api']:
    luftapi_logger.warning("API https not configured")
else:
    if 'port' not in config['api']['https']:
        luftapi_logger.error("API https port not configured")
        exit(1)
    httpsPort = config['api']['https']['port']

    if 'ssl_keyfile' not in config['api']['https']:
        luftapi_logger.error("API https ssl_keyfile not configured")
        exit(1)
    ssl_keyfile = config['api']['https']['ssl_keyfile']

    if 'ssl_keyfile' not in config['api']['https']:
        luftapi_logger.error("API https ssl_keyfile not configured")
        exit(1)
    ssl_certfile = config['api']['https']['ssl_certfile']

    try:
        ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
        ssl_context.load_cert_chain(ssl_certfile, ssl_keyfile)
    except:
        luftapi_logger.error("No SSL Cert or Keyfile provided")
        exit(1)


# Influxdb
if 'influxdb' not in config:
    luftapi_logger.error("InfluxDB not configured")
    exit(1)

if 'host' not in config['influxdb']:
    luftapi_logger.error("InfluxDB host not configured")
    exit(1)
influxdbHost = config['influxdb']['host']

if 'host2' not in config['influxdb']:
    luftapi_logger.error("InfluxDB host2 not configured")
else:
    influxdbHost2 = config['influxdb']['host2']


if 'token' not in config['influxdb']:
    luftapi_logger.error("InfluxDB token not configured")
    exit(1)
influxdbToken = config['influxdb']['token']

if 'token2' not in config['influxdb']:
    luftapi_logger.error("InfluxDB token2 not configured")
else:
    influxdbToken2 = config['influxdb']['token2']


if 'path' not in config['influxdb']:
    luftapi_logger.error("InfluxDB path not configured")
    exit(1)
influxdbPath = config['influxdb']['path']

if 'path2' not in config['influxdb']:
    luftapi_logger.error("InfluxDB path2 not configured")
else:
    influxdbPath2 = config['influxdb']['path2']


# Request
if 'request' not in config:
    luftapi_logger.warning("request not configured")
else:
    if 'proxy' not in config['request']:
        luftapi_logger.warning("request proxy not configured")
    else:
        if 'httpProxy'not in config['request']['proxy']:
            luftapi_logger.error("request proxy httpProxy not configured")
            exit(1)
        httpProxy = config['request']['proxy']['httpProxy']

        if 'httpsProxy' not in config['request']['proxy']:
            luftapi_logger.error("request proxy httpsProxy not configured")
            exit(1)
        httpsProxy = config['request']['proxy']['httpsProxy']

if 'fiware_url' not in config:
    fiware_url=""
else:
    fiware_url = config['fiware_url']

logging.info("------------------------------------------------------------------------------")


@app.post("/sensor")
async def get_body(request: Request):
    error = 0
    input = await request.body()
    input = input.decode("utf-8")
    input = input.split("\n")
    #header = await request.headers.raw()
    print(request.headers.raw)

    #if not await postInfluxdb(input):
    #    error = 1
    # if not await postFirware(input, now)
    #    error = 1
    try:
        Thread(target=postInfluxdb(input)).start()
    except:
        print("PostInflux Error")

    if fiware_url:
        try:
            Thread(target=postFiware(input)).start()
        except Exception as error:
            print("------------------------------------------------------------------------------")
            print("------------------------------------------------------------------------------")
            print(error)
            print("------------------------------------------------------------------------------")

    return JSONResponse(status_code=200, content="item")


def extract_value(label, data_list):
    for item in data_list:
        if label in item:
            # Zerlege den String und suche nach dem numerischen Wert
            parts = item.split()
            try:
                return float(parts[2])
            except ValueError:
                print("float konnte nicht konvertiert werden")

    print(f"Kein Wert gefunden für {label}")
    return None

def postInfluxdb(input):
    output = ""
    temp = input[0].split(" ")

    # 0 = Sensor, 1 = Measurement, 2 = Value, 3 = einheit, 4 = error

    now = str(time.time_ns())
    arduinoID = "arduinoID=" + temp[1]

    for i in input[1:]:
        temp = i.split(" ")
        if temp[0] != "":
            if temp[0] == "START":
                # Measurement
                output = output + "START"
                # Arduino ID
                output = output + "," + arduinoID
                # Einheit
                output = output + ",einheit=" + "START"
                # Error
                if 4 in temp:
                    output = output + ",error=" + "START"
                # Field und Value
                output = output + " " + "START" + "=" + now
                # Zeit und Ende
                output = output + " " + now + "\n"

            # Measurement
            output = output + temp[1]
            # Arduino ID
            output = output + "," + arduinoID
            # Einheit
            output = output + ",einheit=" + temp[3]
            # Error
            if 4 in temp:
                output = output + ",error=" + temp[4]
            # Field und Value
            output = output + " " + temp[0] + "=" + temp[2]
            # Zeit und Ende
            output = output + " " + now + "\n"

    url = influxdbHost + influxdbPath + ''
    headers = {
        "Content-Type": "application/json",
        "Authorization": "Token " + influxdbToken
    }
    if influxdbHost2 and influxdbToken2:
        url2 = influxdbHost2 + influxdbPath2 + ''
        headers2 = {
            "Content-Type": "application/json",
            "Authorization": "Token " + influxdbToken2
        }

    data = output
    proxies = {
        'http': httpProxy,
        'https': httpsProxy,
    }
    if influxdbHost2 and influxdbToken2:
        try:
            response2 = requests.post(url2, data=data, headers=headers2, proxies=proxies)
        except:
            print("An exception occurred")
    
    try:
        response = requests.post(url, data=data, headers=headers, proxies=proxies)
    except:
        print("An exception occurred")
    
    #return (response.ok)
    return

def postFiware(input):
    input = str(input)
    input = ast.literal_eval(input)  # Konvertiere den input String in eine Liste, separiert am Komma

    arduinoID_str = input[0]
    arduinoID = arduinoID_str.split()[1]
    print(arduinoID)
    indoor_env_json["id"] = f"urn:ngsi-ld:IndoorEnvironmentObserved:id:{arduinoID}_BME"
    # Extrahiere Werte für die gewünschten Labels ohne Regex
    temperature = extract_value('BME TEMP', input)
    pressure = extract_value('BME PRES', input)
    humidity = extract_value('BME HUM', input)

    if temperature is not None:
        indoor_env_json["temperature"]["value"] = temperature
        print("temperature added to json")
    else:
        print("missing attribute: temperature")

    if pressure is not None:
        indoor_env_json["atmosphericPressure"]["value"] = pressure / 100  # Umwandlung Pa zu hPa
        print("pressure added to json")
    else:
        print("missing attribute: atmosphericPressure")

    if humidity is not None:
        indoor_env_json["relativeHumidity"]["value"] = humidity / 100  # Prozent zu Dezimal
        print("humidity added to json")
    else:
        print("missing attribute: relativeHumidity")

    print("Final JSON data:", indoor_env_json)
    with requests.Session() as fiware_session:
        fiware_session.verify = False
        response = FiwareLoader(config).updateEntity(indoor_env_json, fiware_session)
        if response.status_code == 404:  # Fehlerbehandlung für Entitäten, die zum Zeitpunkt der patch Anfrage noch nicht existieren.
            # logging.warning(f"Entity {indoor_env_json['id']} didn't exist until now and now gets created with a POST request.")
            print(f"Entity {indoor_env_json['id']} didn't exist until now and now gets created with a POST request.")
            response2 = FiwareLoader(config).sendToFiware(indoor_env_json, fiware_session)
            print(f"{response2.status_code}{response2.text}")
        else:
            print(f"{response.status_code}{response.text}")

def startHttp():
    uvicorn.run(app, host="0.0.0.0", port=httpPort, log_level=loglevel, log_config=log_config)


def startHttps():
    if ssl_certfile and ssl_keyfile:
        uvicorn.run(app, host="0.0.0.0", port=httpsPort, ssl_keyfile=ssl_keyfile, ssl_certfile=ssl_certfile,
                    log_level=loglevel, log_config=log_config)


if __name__ == "__main__":
    p1 = Process(target=startHttp)
    p1.start()
    p2 = Process(target=startHttps)
    p2.start()
    p1.join()
    p2.join()
