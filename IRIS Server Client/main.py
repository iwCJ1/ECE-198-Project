__all__ = ()

from quart import Quart, render_template, request
import config
import db

app = Quart(__name__)

app.secret_key = config.secret_key


@app.template_global()
def appVersion() -> str:
    return 'v1.0'


@app.template_global()
def appName() -> str:
    return 'iSyringe WEB'


@app.errorhandler(404)
async def page_not_found(e):
    # NOTE: we set the 404 status explicitly
    return (await render_template('404.html'), 404)


@app.errorhandler(500)
async def page_not_found(e):
    # NOTE: we set the 500 status explicitly
    return (await render_template('500.html'), 500)


from blueprints.user import user

app.register_blueprint(user, url_prefix='/user')

from blueprints.frontend import frontend

app.register_blueprint(frontend)

if __name__ == '__main__':
    app.run()
