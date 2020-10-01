import sqlite3
class Backend:
    cursor = None
    database = None 
    tableName = None

    def __init__(self):
        
        self.database = self.create_connection("birthdayPresentShop.sqlite3")
        self.tableName = "Catalog"

        if self.database is None:
            print("Unable to conmnect ot database")
        else:
            print("Connected to database Successfully")
            self.initilizeTable(self.tableName)
        
    def close(self): 
        cur = self.database.cursor()
        cur.close()

    def add(self, list):
        res = self.addPointToDB(list, self.tableName)
    
    def commit(self):
        self.database.commit()
    
    def search(self, searchString):
        resp = []
        # commandString = "SELECT * FROM " + self.tableName + " WHERE  id =" + searchString + ";"
        commandStrings = "SELECT * FROM " + self.tableName + "   WHERE  itemName like '%" + searchString + "%';"
        # query = commandStrings.split(;)

        for query in  commandStrings.split(';'): 
            if query != "":
                print ("command Run: " +  query + ";")
                cur = self.database.cursor()
                answer = cur.execute(query + ";").fetchall()
                for ans in answer:
                    resp.append( ans )
                # resp = cur.execute(commandString).fetchall()
                # print (answer)

        return resp

    def questionOne(self, answerString):

        return answerString ==  "testString"

    def questionTwo(self, answerString):
        res = (answerString == "29f4a4d0f6202a0dfa5ec319ab763229" ) 
        # print (" question 2" )
        # print (res )
        return  res

    def questionThree(self, answerString):
        return  self.search(answerString)
 


        



    def show(self):
        return self.getFromDB( self.tableName )

    def delete(self, index):
        print ( type(index) )

        return self.removePointFromDB(index)


    def removePointFromDB(self, index):
        if not isinstance(index, int):
            print ("Unable to remove point. the args is not an interger type") 
            return  None
 
        commandString ="DELETE FROM vector WHERE counter= " + str(index) + ";"
        print (commandString) 
        cur = self.database.cursor()
        resp = cur.execute(commandString)

        return resp
            


    def create_connection(self, db_file):
        """ create a database connection to the SQLite database
            specified by the db_file
        :param db_file: database file
        :return: Connection object or None
        """
        conn = None
        try:
            conn = sqlite3.connect(db_file)
        except Error as e:
            print(e)
            return None
    
        return conn

    def addPointToDB(self, list, tableName):
        if len(list) != 5:
            print ("Unable to add point. Not correct number or parameters") 
            return  None
        
        commandString ="INSERT INTO vector VALUES ('"+str(list[0])+"','"+list[1]+"','"+list[2]+"','"+list[3]+"' ,'"+list[4]+"'  );"
        print (commandString) 
        cur = self.database.cursor()
        resp = cur.execute(commandString)

        return resp
            

    def getFromDB(self, tableName):
        cur = self.database.cursor()
        cur.execute("SELECT * FROM vector;") 
        return cur.fetchall()

    def initilizeTable(self, name):
        cur = self.database.cursor()

        return cur.execute('''
            CREATE TABLE IF NOT EXISTS vector ( 
            counter INT,
            time VARCHAR(100),  
            username VARCHAR(255), 
            point VARCHAR(255), 
            type VARCHAR(255) );''')

