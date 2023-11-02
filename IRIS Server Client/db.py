import pymysql
import config


def checkExist(cid):
    db = pymysql.connect(host=config.host, port=config.port, user=config.user_name, password=config.passwd,
                         database=config.database, charset='utf8')
    print("successfully connected to the database!")
    cursor = db.cursor()
    sql = "SELECT count(*) from client where cid=\"" + str(cid) + "\";"
    cursor.execute(sql)
    cids = cursor.fetchone()
    print(cids[0])
    if cids[0] == 0:
        return 1  # Client doesn't exist
    return 0  # Client exist


def addClient(cid, department):
    if checkExist(cid) == 0:
        return 1  # client id already exist
    db = pymysql.connect(host=config.host, port=config.port, user=config.user_name, password=config.passwd,
                         database=config.database, charset='utf8')
    cursor = db.cursor()
    sql = "INSERT INTO client SET cid=\"" + str(cid) + "\",department=\"" + str(department) + '\";'
    cursor.execute(sql)
    db.commit()
    return 0  # done


def refreshOnlineStat(cid, ip):
    if checkExist(cid) == 1:  # client not ours
        return 1
    db = pymysql.connect(host=config.host, port=config.port, user=config.user_name, password=config.passwd,
                         database=config.database, charset='utf8')
    cursor = db.cursor()
    sql = "UPDATE client SET is_online=1,ip=\"" + str(ip) + "\" WHERE cid=" + str(cid) + ";"
    cursor.execute(sql)
    db.commit()
    return 0  # done


def refreshErrorStat(cid, stat):
    if checkExist(cid) == 1:  # client not ours
        return 1
    db = pymysql.connect(host=config.host, port=config.port, user=config.user_name, password=config.passwd,
                         database=config.database, charset='utf8')
    cursor = db.cursor()
    sql = "UPDATE client SET status=" + str(stat) + " WHERE cid=" + str(cid) + ";"
    cursor.execute(sql)
    db.commit()
    return 0  # done


def checklogin(email, pwd):
    db = pymysql.connect(host=config.host, port=config.port, user=config.user_name, password=config.passwd,
                         database=config.database, charset='utf8')
    print("successfully connected to the database!")
    cursor = db.cursor()
    sql = 'SELECT passwd FROM users WHERE email=' + '"' + email + '"'
    cursor.execute(sql)
    a = cursor.fetchone()
    if a is None:
        print("用户不存在！")
        return 2
    else:
        if a[0] != pwd:
            print("密码错误！")
            return 1
        else:
            print("成功登录！")
            return 0


def refreshavatar(email, newname):
    db = pymysql.connect(host=config.host, port=config.port, user=config.user_name, password=config.passwd,
                         database=config.database, charset='utf8')
    print("successfully connected to the database!")
    cursor = db.cursor()
    sql = 'UPDATE users SET avatar="' + newname + '" WHERE email="' + email + '";'
    cursor.execute(sql)
    db.commit()
    print("OK")
    return 0


def getuserdata(email):
    db = pymysql.connect(host=config.host, port=config.port, user=config.user_name, password=config.passwd,
                         database=config.database, charset='utf8')
    print("successfully connected to the database!")
    cursor = db.cursor()
    sql = 'SELECT user_name FROM users WHERE email=' + '"' + email + '"'
    cursor.execute(sql)
    username = cursor.fetchone()
    sql2 = 'SELECT role FROM users WHERE email=' + '"' + email + '"'
    cursor.execute(sql2)
    is_admin = cursor.fetchone()
    sql4 = 'SELECT avatar FROM users WHERE email=' + '"' + email + '"'
    cursor.execute(sql4)
    avatar = cursor.fetchone()
    sql5 = 'SELECT uid FROM users WHERE email=' + '"' + email + '"'
    cursor.execute(sql5)
    uid = cursor.fetchone()
    return username[0], is_admin[0], avatar[0], uid[0]


def getClientData():
    db = pymysql.connect(host=config.host, port=config.port, user=config.user_name, password=config.passwd,
                         database=config.database, charset='utf8')
    print("successfully connected to the database!")
    cursor = db.cursor()
    sql = 'SELECT count(*) FROM client;'
    cursor.execute(sql)
    allClient = cursor.fetchone()[0]
    sql2 = 'SELECT count(*) FROM client WHERE is_online=1;'
    cursor.execute(sql2)
    onlineClient = cursor.fetchone()
    sql4 = 'SELECT * FROM client;'
    cursor.execute(sql4)
    sheet = cursor.fetchall()
    return allClient, onlineClient, sheet


def checkOnline(cid):
    db = pymysql.connect(host=config.host, port=config.port, user=config.user_name, password=config.passwd,
                         database=config.database, charset='utf8')
    print("successfully connected to the database!")
    cursor = db.cursor()
    sql = "SELECT is_online FROM client WHERE cid=\"" + str(cid) + "\";"
    cursor.execute(sql)
    cids = cursor.fetchone()
    print(cids[0])
    if cids[0] == 1:
        return 1  # online
    return 0  # offline


def getEatDrug(cid):
    db = pymysql.connect(host=config.host, port=config.port, user=config.user_name, password=config.passwd,
                         database=config.database, charset='utf8')
    print("successfully connected to the database!")
    cursor = db.cursor()
    sql = "SELECT eatdrug FROM client WHERE cid=\"" + str(cid) + "\";"
    cursor.execute(sql)
    cids = cursor.fetchone()
    print(cids[0])
    if cids[0] == 1:
        cursor = db.cursor()
        sql2 = 'UPDATE client SET eatdrug = 0 WHERE cid=\"' + str(cid) + "\";"
        cursor.execute(sql2)
        db.commit()
        return '1'  # eat drug
    return '0'  # dont need


def setEatDrug(cid):
    db = pymysql.connect(host=config.host, port=config.port, user=config.user_name, password=config.passwd,
                         database=config.database, charset='utf8')
    print("successfully connected to the database!")
    cursor = db.cursor()
    sql = 'UPDATE client SET eatdrug = 1 WHERE cid=\"' + str(cid) + "\";"
    cursor.execute(sql)
    db.commit()
    return 0  # success
