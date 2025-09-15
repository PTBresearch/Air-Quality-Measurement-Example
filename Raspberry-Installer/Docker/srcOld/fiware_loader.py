# -*- coding: utf-8 -*-

import requests
import yaml
import json
from datetime import datetime, timezone
from urllib.parse import quote
import logging


class FiwareLoader():

    def __init__(self, config):
        self.config = config

    def getFiwareURL(self):
        return self.config["fiware_url"]  

    def deleteEntity(self, entityid): 
        response = requests.delete(url = self.getFiwareURL() + "/" + entityid) 
        return response

    def loadEntity(self, entityid):
        response = requests.get(url = self.getFiwareURL() + "/" + entityid) 
        return response

    def sendToFiware(self, json_data, fiware_session):
        json_data["id"] = json_data["id"].encode('ascii', 'ignore').decode('ascii')

        doubleqoutejson = json.dumps(json_data, indent=2)
        
        url = self.getFiwareURL()
        response = fiware_session.post(url = url, json = json_data, headers = {'Content-Type': 'application/ld+json', 'Connection':'Close'})
        return response

    def updateEntity(self, json_data, fiware_session):
        # Extrahiere die ID aus json_data
        entity_id = json_data["id"]
        # Generiere die URL mit der ID
        url = f"{self.getFiwareURL()}/{entity_id}/attrs"
        # Lösche die ID aus den Daten, weil die bei PATCH nicht mitgesendet werden darf und sende den Rest an Fiware
        json_data_copy = json_data.copy()
        del json_data_copy["id"]
        response = fiware_session.patch(url = url, json = json_data_copy, headers = {'Content-Type': 'application/ld+json', 'Connection':'Close'})
        return response
