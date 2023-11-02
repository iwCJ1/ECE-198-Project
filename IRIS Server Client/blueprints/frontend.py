__all__ = ()

from quart import render_template, redirect, request
from quart import Blueprint
from functools import wraps
from quart import session
from blueprints.utils import flash
import db

frontend = Blueprint('frontend', __name__)


def login_required(func):
    @wraps(func)
    async def wrapper(*args, **kwargs):
        if not session:
            return await flash('error', 'You must be logged in to access that page.', 'login')
        if not session['email']:
            return await flash('error', 'You must be logged in to access that page.', 'login')
        return await func(*args, **kwargs)

    return wrapper


@frontend.route('/')
@frontend.route('/home')
async def hello_world():  # put application's code here
    return await render_template('home.html')


@frontend.route('/login', methods=['GET', "POST"])
async def login():
    if request.method == 'GET':
        return await render_template('login.html')
    user = (await request.form).get('email')
    pwd = (await request.form).get('password')
    print(user)
    print(pwd)
    stat = db.checklogin(str(user), str(pwd))
    if stat == 0:
        username, is_admin, avatar, uid = db.getuserdata(user)
        print(is_admin)
        print(username)
        print(avatar)
        print(uid)

        session['username'] = username
        session['email'] = user
        session['is_admin'] = is_admin
        session['avatar'] = avatar
        session['uid'] = uid
        return await flash('success', '成功登录!', 'home')
    else:
        if stat == 1:
            return await flash('error', 'Password Incorrect', 'login')
        else:
            return await flash('error', 'User does not exist', 'login')


@frontend.route('/client', methods=['GET', 'POST'])
@login_required
async def integral():
    allInte, avaInte, content = db.getClientData()
    print(allInte)
    print(content)
    banInte = allInte - avaInte[0]
    if request.method == 'GET':
        return await render_template('/client.html', content=content, allInte=allInte, banInte=banInte,
                                     avaInte=int(avaInte[0]))
    code = (await request.form).get('code')
    worth = (await request.form).get('worth')
    stat = db.addClient(code, worth)
    if stat == 1:
        return await render_template('/client.html', content=content, allInte=allInte, banInte=banInte,
                                     avaInte=avaInte, flash='Already Exist!', status='error')
    if stat == 0:
        allInte, avaInte, content = db.getClientData()
        banInte = allInte - avaInte[0]
        return await render_template('/client.html', content=content, allInte=allInte, banInte=banInte,
                                     avaInte=int(avaInte[0]), flash='OK!', status='success')


@frontend.route('/alert', methods=['GET', 'POST'])
@login_required
async def alert():
    if request.method == 'GET':
        return await render_template('/alert.html')
    code = (await request.form).get('code')
    stat = db.checkExist(code)
    if stat == 1:
        return await render_template('/alert.html', flash='Client id does not exist!', status='error')
    stat2 = db.checkOnline(code)
    if stat2 == 0:
        return await render_template('/alert.html', flash='Client is not online!', status='error')
    db.setEatDrug(code)
    return await render_template('/alert.html', flash='OK!', status='success')


@frontend.route('/logout')
async def logout_():
    del session['username']
    del session['email']
    del session['is_admin']
    del session['avatar']
    return await flash('success', 'Logout successful!', 'login')


@frontend.route('/iscc')  # iSyringe client connect
async def iscc():
    if 'id' and 'ip' in request.args:
        cid = str(request.args['id'])
        ip = str(request.args['ip'])
        if db.checkExist(cid) == 1:  # doesn't exist
            return '2'
        db.refreshOnlineStat(cid, ip)
        return '1'


@frontend.route('/ises')  # iSyringe client error stat
async def ises():
    if 'id' and 'es' in request.args:
        cid = str(request.args['id'])
        es = str(request.args['ip'])
        if db.checkExist(cid) == 1:  # doesn't exist
            return '2'
        db.refreshErrorStat(cid, es)
        return '1'

@frontend.get('/eatdrug')
async def eatdrug():
    if 'id' in request.args:
        cid = str(request.args['id'])
        if db.checkExist(cid) == 1: # Client doesn't exist
            return '3'
        stat = db.getEatDrug(cid)
        return stat
