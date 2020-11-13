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

class paramType(Enum):
    INT = 0
    STRING = 1

# could be a singleton class
class RobotState():
    def __init__(self, name, value, index, pType, size):
        self.paramNames = {name : name} 
        self.paramVals = {name : value} 
        self.paramIndexes = {name : index} 
        self.paramTypes = {name : pType} 
        self.paramSizes = {name : size} 

        self.dataNames = {name : name} 
        self.dataVals = {name : value} 
        self.dataIndexes = {name : index} 
        self.dataTypes = {name : pType} 
        self.dataSizes = {name : size} 

        self.cmdQ = []
        self.paramUpdateQ = []
        
    def addParam(self, name, value, index, pType, size):
        self.paramNames.update({name : name}) 
        self.paramVals.update({name : value}) 
        self.paramIndexes.update({name : index}) 
        self.paramTypes.update({name : pType}) 
        self.paramSizes.update({name : size}) 

    def addData(self, name, value, index, pType, size):
        self.dataNames = {name : name} 
        self.dataVals = {name : value} 
        self.dataIndexes = {name : index} 
        self.dataTypes = {name : pType} 
        self.dataSizes = {name : size} 

    def updateParamVal(self, name, value):
        self.paramVals.update({name : value}) 

    def updateParamQ(self, name, value):
        self.paramUpdateQ.append((name, value))

    def getParamUpdate(self):
        return self.paramUpdateQ.pop(0)

    def checkParamUpdateQ(self):
        if len(self.paramUpdateQ) > 0:
            return 1

        return 0

    def updateDataVal(self, name, value):
        self.dataVals.update({name : value}) 

    def txCmd(self, cmd):
        self.cmdQ.append(cmd) 

    def rxCmd(self):
        return json.dump({"cmd": self.cmdQ.pop(0)})

registry = RobotState("example", 42, -1, paramType.INT, 4)

class Default(Controller):
  def GET(self):
    return "Hey"

  def POST(self, **kwargs):
    function = kwargs["function"]

    if "botInitParams" in function:
        for i in range(0, int(kwargs["numParams"])):
          registry.addParam(kwargs["p" + str(i)]["name"], kwargs["p" + str(i)]["value"], kwargs["p" + str(i)]["index"], kwargs["p" + str(i)]["type"], kwargs["p" + str(i)]["size"])
            
        #params.update({kwargs["p" + str(i)]["name"]: Param(kwargs["p" + str(i)]["name"], kwargs["p" + str(i)]["value"], paramType.INT, 4)})
        #params.update({kwargs["p" + str(i)]["name"]: kwargs["p" + str(i)]["value"]})
        #params[kwargs["p1"]["name"]] = kwargs["p1"]["value"]
        return registry.paramVals

    if "botGetParams" in function:
        newParamVals = {}
        while len(registry.paramUpdateQ) > 0:
                param = registry.getParamUpdate()
                paramName = param[0]
                paramValue = param[1]
                newParamVals.update({registry.paramIndexes[paramName]: paramValue})

        return dict(zip(newParamVals.keys(), newParamVals.values()))

    if "botInitData" in function:
        for i in range(0, int(kwargs["numData"])):
          registry.addData(kwargs["d" + str(i)]["name"], kwargs["d" + str(i)]["value"], kwargs["d" + str(i)]["index"], kwargs["d" + str(i)]["type"], kwargs["d" + str(i)]["size"])

        return registry.dataVals

    if "botUpdateData" in function:
        registry.updateDataVal(kwargs["name"], kwargs["value"])

        tempDict = registry.dataVals.copy()

#        if checkParamUpdateQ():
#            tempDict.update({"areParamUpdatesAvailable": 1}

        return registry.dataVals

    if "botRxCmd" in function:
        tempDict = {"cmd": registry.rxCmd()}
#        if checkParamUpdateQ():
#            tempDict.update({"areParamUpdatesAvailable": 1})

#        return json.dumps({"cmd": registry.rxCmd()})
        return tempDict 

    if "uiGetParams" in function:
        return registry.paramVals

    if "uiUpdateParam" in function:
        registry.updateParamQ(kwargs["name"], kwargs["value"])

        return registry.paramUpdateQ

    if "uiGetData" in function:
        return registry.dataVals

    if "uiTxCmd" in function:
        registry.txCmd(kwargs["cmd"])

#    if "set_mosi" in function or "SET_MOSI" in function:
#      print(kwargs["args"])
#      if kwargs["args"]["running"]:
#        params["running"] =  1
#
#      else:
#        params["running"] =  0
#
#      return params
#
#    if "check_running" in function or "CHECK_RUNNING" in function:
#      if params["running"] is 0:
#        return "eslaf"
#      else:
#        return  "eurt"
#
#    if "miso" in function or "MISO" in function:
#      return params
#
#    if "MOSI" in function or "MOSI" in function:
#      return params
#
#
#    if "setParams_params" in function or "GET_PARAMS" in function:
#      # backend.add(args)
#      # backend.commit()
#      return lastestUpdate
#
#    if "check_latest_change" in function or "CHECK_LATEST_CHANGE" in function:
#      return lastestUpdate
      
