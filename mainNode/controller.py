
from endpoints import Controller
# import Backend 
import json
from json import JSONEncoder
from enum import Enum

class Active(Enum):
  stop = 0
  run = 1

class Mode(Enum):
  pwm = 0
  position = 1
  velocity = 2

lastestUpdate = None
params =  { "running": 0, "mode": 0, "PWMDuty": 0 }


class Default(Controller):
  def GET(self):
    return "Hey"

  def POST(self, **kwargs):
    function = kwargs["function"]
    # args = kwargs["args"]

    if "get_params" in function or "GET_PARAMS" in function:
      return params

    if "set_mosi" in function or "SET_MOSI" in function:
      print(kwargs["args"])
      if kwargs["args"]["running"]:
        params["running"] =  1

      else:
        params["running"] =  0

      return params



    if "check_running" in function or "CHECK_RUNNING" in function:
      if params["running"] is 0:
        return "eslaf"
      else:
        return  "eurt"

    if "miso" in function or "MISO" in function:
      return params

    if "MOSI" in function or "MOSI" in function:
      return params


    if "setParams_params" in function or "GET_PARAMS" in function:
      # backend.add(args)
      # backend.commit()
      return lastestUpdate

    if "check_latest_change" in function or "CHECK_LATEST_CHANGE" in function:
      return lastestUpdate
 
      
