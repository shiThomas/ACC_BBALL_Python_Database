#include <stdio.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <pqxx/pqxx>
#include <sstream>
#include <string>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

void drop_table(connection * C) {
  string sql_droptable = "DROP TABLE IF EXISTS PLAYER, TEAM, STATE, COLOR;";
  work Droptable(*C);
  Droptable.exec(sql_droptable);
  Droptable.commit();
}

void create_table(connection * C) {
  string sql_createplayer;
  string sql_createteam;
  string sql_createstate;
  string sql_createcolor;
  sql_createcolor = "CREATE TABLE COLOR("
                    "COLOR_ID        SERIAL          PRIMARY KEY,"
                    "NAME            TEXT            NOT NULL);";

  sql_createstate = "CREATE TABLE STATE("
                    "STATE_ID        SERIAL          PRIMARY KEY,"
                    "NAME            TEXT            NOT NULL);";

  sql_createteam = "CREATE TABLE TEAM("
                   "TEAM_ID          SERIAL          PRIMARY KEY,"
                   "NAME             TEXT            NOT NULL,"
                   "STATE_ID         INT             REFERENCES STATE(STATE_ID),"
                   "COLOR_ID         INT             REFERENCES COLOR(COLOR_ID),"
                   "WINS             INT             NOT NULL,"
                   "LOSSES           INT             NOT NULL);";

  sql_createplayer = "CREATE TABLE PLAYER("
                     "PLAYER_ID      SERIAL                PRIMARY KEY,"
                     "TEAM_ID        INT                   REFERENCES TEAM(TEAM_ID),"
                     "UNIFORM_NUM    INT                   NOT NULL,"
                     "FIRST_NAME     TEXT                  NOT NULL,"
                     "LAST_NAME      TEXT                  NOT NULL,"
                     "MPG            INT                   NOT NULL,"
                     "PPG            INT                   NOT NULL,"
                     "RPG            INT                   NOT NULL,"
                     "APG            INT                   NOT NULL,"
                     "SPG            DOUBLE PRECISION      NOT NULL,"
                     "BPG            DOUBLE PRECISION      NOT NULL);";

  work Createtable(*C);
  Createtable.exec(sql_createstate);
  Createtable.exec(sql_createcolor);
  Createtable.exec(sql_createteam);
  Createtable.exec(sql_createplayer);

  Createtable.commit();
}

void import_info(const char * input_file, connection * C) {
  ifstream fin(input_file, ifstream::in);

  //player
  int player_id, team_id, uniform_num, mpg, ppg, rpg, apg;
  string first_name, last_name;
  double spg, bpg;
  //team
  int tm_id, state_id, color_id, wins, losses;
  string team_name;
  //state
  int st_id;
  string state_name;
  //color
  int clr_id;
  string color_name;
  //operation

  if (!strcmp(input_file, "player.txt")) {
    while (!fin.eof()) {
      string curr;
      getline(fin, curr);
      stringstream ss(curr);
      ss >> player_id >> team_id >> uniform_num >> first_name >> last_name >> mpg >> ppg >> rpg >>
          apg >> spg >> bpg;
      add_player(C, team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
    }
  }
  if (!strcmp(input_file, "team.txt")) {
    while (!fin.eof()) {
      string curr;
      getline(fin, curr);
      stringstream ss(curr);
      ss >> tm_id >> team_name >> state_id >> color_id >> wins >> losses;

      string team_info;
      add_team(C, team_name, state_id, color_id, wins, losses);
    }
  }
  if (!strcmp(input_file, "state.txt")) {
    while (!fin.eof()) {
      string curr;
      getline(fin, curr);
      stringstream ss(curr);
      ss >> st_id >> state_name;
      string state_info;
      add_state(C, state_name);
    }
  }
  if (!strcmp(input_file, "color.txt")) {
    while (!fin.eof()) {
      string curr;
      getline(fin, curr);
      stringstream ss(curr);
      ss >> clr_id >> color_name;
      string color_info;
      add_color(C, color_name);
    }
  }
  fin.close();
}

int main(int argc, char * argv[]) {
  //Allocate & initialize a Postgres connection object
  connection * C;

  try {
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      //  cout << "Opened database successfully: " << C->dbname() << endl;
    }
    else {
      cout << "Can't open database" << endl;
      return 1;
    }

    drop_table(C);
    create_table(C);
    import_info("state.txt", C);
    import_info("color.txt", C);
    import_info("team.txt", C);
    import_info("player.txt", C);
  }
  catch (const std::exception & e) {
    cerr << e.what() << std::endl;
    return 1;
  }

  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files

  exercise(C);

  //Close database connection
  C->disconnect();

  return 0;
}
