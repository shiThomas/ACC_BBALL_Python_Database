#include "query_funcs.h"

void add_player(connection * C,
                int team_id,
                int jersey_num,
                string first_name,
                string last_name,
                int mpg,
                int ppg,
                int rpg,
                int apg,
                double spg,
                double bpg) {
  work addplayer(*C);

  char player_info[512];
  sprintf(player_info,
          "INSERT INTO PLAYER (TEAM_ID,UNIFORM_NUM,FIRST_NAME,"
          "LAST_NAME,MPG,PPG,RPG,APG,SPG,BPG) VALUES ( %d, %d, %s, %s, %d, %d, %d, %d, %lf, "
          "%lf);",
          team_id,
          jersey_num,
          addplayer.quote(first_name).c_str(),
          addplayer.quote(last_name).c_str(),
          mpg,
          ppg,
          rpg,
          apg,
          spg,
          bpg);
  addplayer.exec(player_info);
  addplayer.commit();
}

void add_team(connection * C, string name, int state_id, int color_id, int wins, int losses) {
  work addteam(*C);
  char team_info[256];
  sprintf(team_info,
          "INSERT INTO TEAM (NAME,STATE_ID,COLOR_ID,WINS,LOSSES) VALUES (%s, %d, %d, %d, %d);",
          addteam.quote(name).c_str(),
          state_id,
          color_id,
          wins,
          losses);

  addteam.exec(team_info);
  addteam.commit();
}

void add_state(connection * C, string name) {
  work addstate(*C);
  char state_info[256];
  sprintf(state_info, "INSERT INTO STATE (NAME) VALUES (%s);", addstate.quote(name).c_str());

  addstate.exec(state_info);
  addstate.commit();
}

void add_color(connection * C, string name) {
  work addcolor(*C);

  char color_info[256];
  sprintf(color_info, "INSERT INTO COLOR (NAME) VALUES (%s);", addcolor.quote(name).c_str());
  addcolor.exec(color_info);
  addcolor.commit();
}

void query1(connection * C,
            int use_mpg,
            int min_mpg,
            int max_mpg,
            int use_ppg,
            int min_ppg,
            int max_ppg,
            int use_rpg,
            int min_rpg,
            int max_rpg,
            int use_apg,
            int min_apg,
            int max_apg,
            int use_spg,
            double min_spg,
            double max_spg,
            int use_bpg,
            double min_bpg,
            double max_bpg) {
  work plyr_stats(*C);
  bool used = false;
  string sql = "SELECT * FROM PLAYER ";
  if (use_mpg) {
    sql += "WHERE MPG >= " + to_string(min_mpg) + " AND MPG <= " + to_string(max_mpg) + " ";
    used = true;
  }
  if (use_ppg) {
    if (used) {
      sql += "AND ";
    }
    else {
      sql += "WHERE ";
      used = true;
    }
    sql += "PPG >= " + to_string(min_ppg) + " AND PPG <= " + to_string(max_ppg) + " ";
  }
  if (use_rpg) {
    if (used) {
      sql += "AND ";
    }
    else {
      sql += "WHERE ";
      used = true;
    }
    sql += "RPG >= " + to_string(min_rpg) + " AND RPG <= " + to_string(max_rpg) + " ";
  }
  if (use_apg) {
    if (used) {
      sql += "AND ";
    }
    else {
      sql += "WHERE ";
      used = true;
    }
    sql += "APG >= " + to_string(min_apg) + " AND APG <= " + to_string(max_apg) + " ";
  }
  if (use_spg) {
    if (used) {
      sql += "AND ";
    }
    else {
      sql += "WHERE ";
      used = true;
    }
    sql += "SPG >= " + to_string(min_spg) + " AND SPG <= " + to_string(max_spg) + " ";
  }
  if (use_bpg) {
    if (used) {
      sql += "AND ";
    }
    else {
      sql += "WHERE ";
      used = true;
    }
    sql += "BPG >= " + to_string(min_bpg) + " AND BPG <= " + to_string(max_bpg) + " ";
  }
  sql += ";";
  result rslt(plyr_stats.exec(sql));
  cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;
  for (result::const_iterator it = rslt.begin(); it != rslt.end(); ++it) {
    cout << it[0].as<int>() << " " << it[1].as<int>() << " " << it[2].as<int>() << " "
         << it[3].as<string>() << " " << it[4].as<string>() << " " << it[5].as<int>() << " "
         << it[6].as<int>() << " " << it[7].as<int>() << " " << it[8].as<int>() << " ";
    cout.setf(ios::fixed);
    cout << setprecision(1) << it[9].as<double>() << " ";
    cout << setprecision(1) << it[10].as<double>() << endl;
  }
}

void query2(connection * C, string team_color) {
  work team_info(*C);
  string sql =
      "SELECT TEAM.NAME FROM TEAM, COLOR WHERE TEAM.COLOR_ID=COLOR.COLOR_ID AND COLOR.NAME=" +
      team_info.quote(team_color) + ";";
  result rslt(team_info.exec(sql));
  cout << "NAME" << endl;
  for (result::const_iterator it = rslt.begin(); it != rslt.end(); ++it) {
    cout << it[0].as<string>() << endl;
  }
}

void query3(connection * C, string team_name) {
  work player_name(*C);
  string sql = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME FROM PLAYER,TEAM WHERE "
               "PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.NAME=" +
               player_name.quote(team_name) + "ORDER BY PPG DESC;";
  result rslt(player_name.exec(sql));
  cout << "FIRST_NAME LAST_NAME" << endl;
  for (result::const_iterator it = rslt.begin(); it != rslt.end(); ++it) {
    cout << it[0].as<string>() << " ";
    cout << it[1].as<string>() << endl;
  }
}

void query4(connection * C, string team_state, string team_color) {
  work player_info(*C);
  string sql =
      "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME,PLAYER.UNIFORM_NUM FROM PLAYER, TEAM, "
      "STATE,COLOR WHERE PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.STATE_ID=STATE.STATE_ID AND "
      "STATE.NAME=" +
      player_info.quote(team_state) +
      " AND TEAM.COLOR_ID = COLOR.COLOR_ID AND COLOR.NAME=" + player_info.quote(team_color) + ";";
  result rslt(player_info.exec(sql));
  cout << "FIRST_NAME LAST_NAME UNIFORM_NUM" << endl;
  for (result::const_iterator it = rslt.begin(); it != rslt.end(); ++it) {
    cout << it[0].as<string>() << " ";
    cout << it[1].as<string>() << " ";
    cout << it[2].as<int>() << endl;
  }
}

void query5(connection * C, int num_wins) {
  work p(*C);
  string sql = "SELECT PLAYER.FIRST_NAME,PLAYER.LAST_NAME,TEAM.NAME,TEAM.WINS FROM PLAYER, TEAM "
               "WHERE PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.WINS>" +
               to_string(num_wins) + ";";
  result rslt(p.exec(sql));
  cout << "FIRST_NAME LAST_NAME NAME WINS" << endl;
  for (result::const_iterator it = rslt.begin(); it != rslt.end(); ++it) {
    cout << it[0].as<string>() << " ";
    cout << it[1].as<string>() << " ";
    cout << it[2].as<string>() << " ";
    cout << it[3].as<int>() << endl;
  }
}
