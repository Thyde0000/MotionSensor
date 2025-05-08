import json
import os
import urllib.request
import pymysql

def lambda_handler(event, context):
    print("Lambda function STARTED.")
    print(f"Incoming event: {json.dumps(event)}")
    
    #first try to extract message
    try:

        body = json.loads(event['body'])
        msg = body.get('message', 'Motion detected')
    except Exception as e:
        print(f"Error parsing event body: {str(e)}")
        msg = 'Motion detected'  #fallback message

    # send Telegram Message
    try:
        telegram_url = f"https://api.telegram.org/bot{os.environ['TELEGRAM_BOT_TOKEN']}/sendMessage"
        payload = json.dumps({
            "chat_id": os.environ['TELEGRAM_CHAT_ID'],
            "text": msg
        }).encode('utf8')
        
        req = urllib.request.Request(telegram_url, data=payload, headers={'Content-Type': 'application/json'})
        
        with urllib.request.urlopen(req) as response:
            response_body = response.read().decode('utf8')
            print(f"Telegram Response: {response.status}, {response_body}")
            
    except Exception as e:
        print(f"Telegram send failed: {str(e)}")
    
    # insert into RDS Database
    try:
        conn = pymysql.connect(
            host=os.environ['DB_HOST'],
            user=os.environ['DB_USER'],
            password=os.environ['DB_PASS'],
            database=os.environ['DB_NAME'],
            port=3306
        )
        cur = conn.cursor()
        insert_query = "INSERT INTO motion_events (sensor_status) VALUES ('MOTION_DETECTED')"
        cur.execute(insert_query)
        conn.commit()
        cur.close()
        conn.close()
        print("Inserted motion event into RDS database.")
    except Exception as e:
        print(f"Database insert failed: {str(e)}")
        
  
    return {
        'statusCode': 200,
        'body': json.dumps({'status': 'logged', 'message': msg})
    }
