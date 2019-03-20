from sqlalchemy import create_engine
from sqlalchemy import ForeignKey, desc

from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, String, Integer, Float
from sqlalchemy.orm import sessionmaker, relationship
engine = create_engine('postgresql://postgres:passw0rd@localhost:5432/ACC_BBALL')
Base = declarative_base()

class Color(Base):

    __tablename__= 'color'

    color_id= Column(Integer,primary_key=True)
    name = Column(String(64),nullable=False)


class State(Base):

    __tablename__='state'

    state_id = Column(Integer,primary_key=True)
    name = Column(String(64),nullable = False)


class Team(Base):
    __tablename__='team'

    team_id = Column(Integer,primary_key=True)
    name = Column(String(64),nullable = False)
    state_id = Column(Integer,ForeignKey('state.state_id'))
    color_id = Column(Integer,ForeignKey('color.color_id'))
    wins = Column(Integer,nullable = False)
    losses = Column(Integer,nullable = False)


class Player(Base):
    __tablename__='player'

    player_id = Column(Integer,primary_key=True)
    team_id = Column(Integer,ForeignKey('team.team_id'))
    uniform_num = Column(Integer,nullable = False)
    first_name = Column(String(64),nullable = False)
    last_name = Column(String(64),nullable = False)
    uniform_num = Column(Integer,nullable = False)
    mpg = Column(Integer,nullable = False)
    ppg = Column(Integer,nullable = False)
    rpg = Column(Integer,nullable = False)
    apg = Column(Integer,nullable = False)
    spg = Column(Float,nullable = False)
    bpg = Column(Float,nullable = False)


Session = sessionmaker(bind=engine)
session = Session()
    
    
def query1( use_mpg,min_mpg, max_mpg, use_ppg, min_ppg, max_ppg, use_rpg, min_rpg, max_rpg, use_apg, min_apg, max_apg, use_spg, min_spg, max_spg, use_bpg,min_bpg,max_bpg):

    query=session.query(Player)
    if use_mpg is not 0:
        query = query.filter(Player.mpg>=min_mpg, Player.mpg<=max_mpg)
        
    if use_ppg is not 0:
       
        query = query.filter(Player.ppg>=min_ppg, Player.ppg<=max_ppg)
        
    if use_rpg is not 0:
        query = query.filter(Player.rpg>=min_rpg , Player.rpg<=max_rpg)

    if use_apg is not 0:
        query = query.filter(Player.apg>=min_apg , Player.apg<=max_apg)

    if use_spg is not 0:
        query = query.filter(Player.spg>=min_spg , Player.spg<=max_spg)

    if use_bpg is not 0:
        query = query.filter(Player.bpg>=min_bpg , Player.bpg<=max_bpg)

        

    print("PLAYER_ID","TEAM_ID","UNIFORM_NUM","FIRST_NAME","LAST_NAME","MPG","PPG","RPG","APG","SPG","BPG")
    for element in query:
       print(element.player_id,element.team_id,element.uniform_num,element.first_name,element.last_name,element.mpg,element.ppg,element.rpg,element.apg,element.spg,element.bpg)


def query2(team_color):
    query = session.query(Team,Color).filter(Team.color_id == Color.color_id,Color.name == team_color)
    print("NAME")
    for element in query:
        print(element.Team.name)

def query3(team_name):
    query = session.query(Player,Team).filter(Player.team_id ==Team.team_id,Team.name == team_name).order_by(desc(Player.ppg))
    print("FIRST_NAME","LAST_NAME")
    for element in query:
        print(element.Player.first_name,element.Player.last_name)

def query4(team_state,team_color):
    query = session.query(Team,Color,Player).filter(Player.team_id == Team.team_id,Team.state_id==State.state_id,State.name==team_state,Team.color_id==Color.color_id,Color.name ==team_color)
    print("FIRST_NAME","LAST_NAME","UNIFORM_NUM")
    for element in query:
        print(element.Player.first_name,element.Player.last_name,element.Player.uniform_num)


def query5(num_wins):
    query = session.query(Player,Team).filter(Player.team_id==Team.team_id,Team.wins>num_wins)
    print("FIRST_NAME","LAST_NAME","NAME","WINS")
    for element in query:
        print(element.Player.first_name,element.Player.last_name,element.Team.name,element.Team.wins)
        
query1( 1, 0, 30, 1, 10, 30, 1, 10, 20, 0, 0, 0, 0, 0.0, 5.0, 0, 0.0, 5.0)
query2('DarkBlue')
query3("Duke")
query4("NC","DarkBlue")
query5(11)
