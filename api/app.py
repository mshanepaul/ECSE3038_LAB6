from flask import Flask, request, jsonify
from flask_pymongo import PyMongo
from marshmallow import Schema, fields, ValidationError
from bson.json_util import dumps
from flask_cors import CORS
from json import loads
from keys import keys
import datetime

app = Flask(__name__)
CORS(app)

app.config["MONGO_URI"] = "mongodb+srv://admin:"+keys["pw"]+"@cluster0.41j7h.mongodb.net/"+keys["nm"]+"?retryWrites=true&w=majority"
mongo = PyMongo(app)

db_operations = mongo.db.tanks
dte = datetime.datetime.now()

class Level(Schema):
    tank_id = fields.Integer(required=True)
    percentage_full = fields.Integer(required=True)

@app.route("/", methods=["GET"])
def home():
    return "hello lab 6"

# TANK ROUTE:
@app.route("/tank", methods=["POST"])
def postTankData():
    try:
        tank_id = request.json["tank_id"]
        water_level = request.json["water_level"]
        percentage_full = ((water_level - 10) / 190) * 100

        jsonBody = {
            "tank_id": tank_id,
            "percentage_full": percentage_full
        }
        
        newTank = Level().load(jsonBody)

        db_operations.insert_one(newTank)
        
        return{
            "success": True,
            "msg": "data saved in database successfully",
            "date": dte.strftime("%c")
        }
    except ValidationError as e:
        return e.messages, 400

# Main
if __name__ == '__main__':
   app.run(debug = True, host="192.168.100.82", port="5000")
