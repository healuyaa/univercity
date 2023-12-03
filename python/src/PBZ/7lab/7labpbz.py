from flask import Flask, render_template
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import DECIMAL
from flask_admin import Admin
from flask_admin.contrib.sqla import ModelView

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://root:1234@localhost/lab567'
app.config['SECRET_KEY'] = 'your_secret_key'
db = SQLAlchemy(app)
admin = Admin(app)

class table1(db.Model):
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    code_model = db.Column(db.String(10), nullable=True)
    model = db.Column(db.String(10), nullable=True)
    car_model = db.Column(db.String(10), nullable=True)
    car_manufacturer = db.Column(db.String(20), nullable=True)

    def __repr__(self):
        return f'<table1(id={self.id}, code_model={self.code_model}, model={self.model}, car_model={self.car_model}, car_manufacturer={self.car_manufacturer})>'

class table2(db.Model):
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    code_model = db.Column(db.String(10), nullable=True)
    number_model = db.Column(db.Integer, nullable=True)
    color = db.Column(db.String(10), nullable=True)
    mileage = db.Column(db.Integer, nullable=True)
    price = db.Column(DECIMAL(precision=10, scale=2), nullable=True)

    def __repr__(self):
        return f'<table2(id={self.id}, code_model={self.code_model}, number_model={self.number_model}, color={self.color}, milieage={self.mileage}, price={self.price})>'

class table3(db.Model):
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    number_model = db.Column(db.Integer, nullable=True)
    year_of_issue = db.Column(db.Integer, nullable=True)
    engine_capacity = db.Column(db.Integer, nullable=True)

    def __repr__(self):
        return f'<table3(id={self.id}, number_model={self.number_model}, year_of_issue={self.year_of_issue}, engine_capacity={self.engine_capacity})>'

class table1AdminView(ModelView):
    column_display_pk = True

class table2AdminView(ModelView):
    column_display_pk = True

class table3AdminView(ModelView):
    column_display_pk = True

admin.add_view(table1AdminView(table1, db.session))
admin.add_view(table2AdminView(table2, db.session))
admin.add_view(table3AdminView(table3, db.session))

@app.route('/')
def index():
    with app.app_context():
        data_table1 = table1.query.all()
        data_table2 = table2.query.all()
        data_table3 = table3.query.all()
    return render_template('index.html', data_table1=data_table1, data_table2=data_table2, data_table3=data_table3)

if __name__ == '__main__':
    app.run()
