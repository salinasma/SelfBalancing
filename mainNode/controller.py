
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
  #   return backend.show()

  def POST(self, **kwargs):
    function = kwargs["function"]
    args = kwargs["args"]

    if "get_params" in function or "GET_PARAMS" in function:
      # backend.add(args)
      # backend.commit()
      return params

    if "setParams_params" in function or "GET_PARAMS" in function:
      # backend.add(args)
      # backend.commit()
      return lastestUpdate

    if "check_latest_change" in function or "CHECK_LATEST_CHANGE" in function:
      return lastestUpdate
 
      
    # if "print" in function or "PRINT" in function or "show" in function:
    #   return backend.show()

    # if "CLOSE" in function or "close" in function:
    #   backend.close()
    #   return 'close'

    # if "delete" in function:
    #   backend.delete(args)
    #   backend.commit()
    #   return  'delete'