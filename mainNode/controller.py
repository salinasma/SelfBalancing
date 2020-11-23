from endpoints import Controller

import json
from json import JSONEncoder
from enum import Enum

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
        self.dataNames.update({name : name})
        self.dataVals.update({name : value}) 
        self.dataIndexes.update({name : index})
        self.dataTypes.update({name : pType}) 
        self.dataSizes.update({name : size}) 

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

registry = RobotState("example", 42, -1, 0, 4) # The 0 indicates type INT

class Default(Controller):
  def GET(self):
    return "Hey"

  def POST(self, **kwargs):
    function = kwargs["function"]

    if "botInitParams" in function:
        for i in range(0, int(kwargs["numParams"])):
            registry.addParam(kwargs["p" + str(i)]["name"], (int)(kwargs["p" + str(i)]["value"]), kwargs["p" + str(i)]["index"], kwargs["p" + str(i)]["type"], kwargs["p" + str(i)]["size"])

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

        return registry.dataVals

    if "botRxCmd" in function:
        return json.dumps({"cmd": registry.rxCmd()})

    if "uiGetParams" in function:
        tempDict = {}
        for key in registry.paramVals.keys():
            tempDict.update({key: {"value": registry.paramVals[key], "dataType": registry.paramTypes[key]}})

        return  tempDict 

    if "uiUpdateParam" in function:
        registry.updateParamQ(kwargs["name"], kwargs["value"])

        return registry.paramUpdateQ

    if "uiGetData" in function:
        return registry.dataVals

    if "uiTxCmd" in function:
        registry.txCmd(kwargs["cmd"])
      
