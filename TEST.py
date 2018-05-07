from flask import Flask, render_template, request,session
from datetime import datetime
import psycopg2
import sys
from time import gmtime, strftime

app = Flask(__name__)

@app.route('/')
def home():  
    return render_template('index.html')
@app.route('/displayQ4')
def displayQ4():  
    return render_template('index.html')
@app.route('/displayQ7')
def displayQ7():  
    return render_template('index.html')
  
#Called when the user submits data for questions 1
@app.route('/ExQ1', methods=['GET'])
def ExQ1():
    customerID=str(request.args['customerId'])
    name=str(request.args['customerName'])
    emailAddress=str(request.args['customerEmail'])

    query="INSERT INTO customer (customerid, name, email) VALUES ('"+customerID+"','"+name+"','"+emailAddress+"')"

    try:
        exQuery(query,False)
        return render_template('index.html',message="Inserted new customer "+name+"into the database")       
    except Exception as e:
        return render_template('index.html', error=e,message="Could not insert new customer in to database")

#Called when the user submits data for questions 2
@app.route('/ExQ2', methods=['GET'])
def ExQ2():
    productName=str(request.args['productName'])
    problem=str(request.args['problem'])
    priority=str(request.args['priority'])
    customerID=str(request.args['customerID'])
    currentTime= strftime("%Y-%m-%d %H:%M:%S", gmtime())   

    try:
        query="select * from ticket"
        rows=exQuery(query,False)
        print(rows)
        newTicketID=str(len(rows))     
        print(newTicketID)
        query="INSERT INTO ticket (ticketID,problem,status,priority,loggedtime,customerid,productid) VALUES ("+newTicketID+",'"+problem+"','open',"+priority+",'"+currentTime+"',"+customerID+","+productName+")"
        exQuery(query,False)       
        return render_template('index.html')       
    except Exception:
        return render_template('index.html')

#Called when the user submits data for questions 3
@app.route('/ExQ3', methods=['GET'])
def ExQ3():


    staffID=str(request.args['staffId'])
    updateText=str(request.args['updateText'])
    ticketID=str(request.args['ticketID'])
    currentTime= strftime("%Y-%m-%d %H:%M:%S", gmtime())

    try:       
        query="select * from ticketUpdate"
        newTicketupdteID=str(len(exQuery(query,False))+1)
        print(newTicketupdteID)
        query="INSERT INTO ticketupdate (ticketupdateid,message,updatetime,ticketid,staffid) VALUES ("+newTicketupdteID+",'"+updateText+"','"+currentTime+"',"+ticketID+","+staffID+")"
        exQuery(query,False)
        return render_template('index.html')
    except Exception:
        return render_template('index.html')

#Called when the user submits data for questions 4
@app.route('/ExQ4', methods=['GET'])
def ExQ4():
    query="SELECT ticket.ticketid, MAX (ticketupdate.updatetime) AS lastUpdate \
    From ticket,ticketupdate WHERE ticket.status='open' AND ticket.ticketid=ticketupdate.ticketid \
    GROUP BY  ticket.ticketid ORDER BY ticket.ticketid"

    rows=exQuery(query,False)
    return render_template('displayTable.html', rows=rows)        

#Called when the user submits data for questions 5
@app.route('/ExQ5', methods=['GET'])
def ExQ5():
    ticketID=str(request.args['ticketID'])
    query="UPDATE ticket SET  status ='closed' WHERE ticket.ticketid="+ticketID
    try:
        exQuery(query,False)
        return render_template('index.html')       
    except Exception as e:
        return render_template('index.html', error=e)

@app.route('/ExQ6', methods=['GET'])
def ExQ6():
    ticketID=str(request.args['ticketID'])
    query="SELECT ticket.problem,ticketupdate.message,ticketupdate.updatetime,ticketupdate.staffid \
            FROM ticket,ticketupdate \
            WHERE ticket.ticketid=1 AND ticketupdate.ticketid='"+ticketID+"'\
            ORDER BY ticketupdate.updatetime ASC;"
    print(query)   

    try:
        conn=None
        conn=getConnection()  
        cur = conn.cursor()         
        cur.execute('SET search_path to public')

        #The SQL statement
        cur.execute(query)        
        
        conn.commit()
        rows=cur.fetchall()
        return render_template('displayTable.html', rows=rows)          
    except Exception as e:
        return render_template('index.html', error=e)

@app.route('/ExQ7', methods=['GET'])
def ExQ7():
    query="SELECT t.ticketid, Count(tu.ticketid),  MIN(tu.updatetime::timestamp)-t.loggedtime::timestamp AS MinTime, MAX(tu.updatetime::timestamp)-t.loggedtime::timestamp as MaxTime\
            From\
            ticket t LEFT JOIN\
            ticketupdate tu on t.ticketid=tu.ticketid\
            WHERE t.status='closed'\
            Group by t.ticketid ORDER BY \
            t.ticketid ASC"
    try:
        rows=exQuery(query,False)
        return render_template('displayTable.html', rows=rows)          
    except Exception as e:
        return render_template('index.html', error=e)
           

@app.route('/ExQ8', methods=['GET'])
def ExQ8():
    customerID=str(request.args['customerId'])
    query="DELETE FROM customer cst\
        WHERE cst.customerid="+customerID+"AND NOT EXISTS ( \
        SELECT 1 FROM ticket tk WHERE cst.customerid=tk.customerid)"
    try:
        exQuery(query,False)
        return render_template('index.html', message="Customer deleted")          
    except Exception as e:
        print(e)
        return render_template('index.html', error=e)

def getConnection():
    pwFile = open("pw.txt", "r")
    pw = pwFile.read()
    pwFile.close()
    conn_string = "host='cmpstudb-01.cmp.uea.ac.uk' dbname='ufg17jku' user='ufg17jku' password=" +pw
    conn = psycopg2.connect(conn_string)
    return conn

def exQuery(query,getRowCount):
    conn=None
    conn=getConnection()  
    cur = conn.cursor()         
    cur.execute('SET search_path to public')
    cur.execute(query) 
    conn.commit()
    return cur.fetchall()

if __name__ == "__main__":
	app.run(debug=True)
