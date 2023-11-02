__all__ = ()

# user模块
from quart import Blueprint
from quart import redirect
from quart import render_template
from quart import request
from quart import session
from quart import send_file
from functools import wraps
import os
from blueprints.utils import flash
import db
import time
from PIL import Image
from blueprints.utils import crop_image

user = Blueprint('user', __name__)


def login_required(func):
    @wraps(func)
    async def wrapper(*args, **kwargs):
        if not session:
            return await flash('error', 'You must be logged in to access that page.', 'login')
        if not session['email']:
            return await flash('error', 'You must be logged in to access that page.', 'login')
        return await func(*args, **kwargs)

    return wrapper


# user模块
@user.route('/')
@user.route('/home')
@login_required
async def userhome():
    integ = 1
    shopping = 1
    return await render_template('/user/home.html', integ=integ, shopping=shopping)


@user.route('/avatar', methods=['GET', 'POST'])
@login_required
async def avatar():
    if request.method == 'GET':
        return await render_template('/user/avatar.html')
    ALLOWED_EXTENSIONS = ['.jpeg', '.jpg', '.png']
    avatara = (await request.files).get('avatar')
    if avatara is None or not avatara.filename:
        return await flash('error', 'No image was selected!', '/user/avatar')
    filename, file_extension = os.path.splitext(avatara.filename.lower())
    # bad file extension; deny post
    if not file_extension in ALLOWED_EXTENSIONS:
        return await flash('error', 'The image you select must be either a .JPG, .JPEG, or .PNG file!',
                           '/user/avatar')
    pilavatar = Image.open(avatara.stream)
    pilavatar = crop_image(pilavatar)
    newname = str(session['uid'] + int(time.time())) + '.jpg'
    pilavatar.convert("RGB").save(os.path.join('static/avatars', newname))
    db.refreshavatar(session['email'], newname)
    del session['avatar']
    username, is_admin, avatar, uid = db.getuserdata(session['email'])
    session['avatar'] = avatar
    return await flash('success', 'Successfully uploaded the new avatar', '/user/avatar')


