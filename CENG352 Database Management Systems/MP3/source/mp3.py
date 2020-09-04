from customer import Customer

import psycopg2

from config import read_config
from messages import *

POSTGRESQL_CONFIG_FILE_NAME = "database.cfg"

"""
    Connects to PostgreSQL database and returns connection object.
"""


def connect_to_db():
    db_conn_params = read_config(filename=POSTGRESQL_CONFIG_FILE_NAME, section="postgresql")
    conn = psycopg2.connect(**db_conn_params)
    conn.autocommit = False
    return conn


"""
    Splits given command string by spaces and trims each token.
    Returns token list.
"""


def tokenize_command(command):
    tokens = command.split(" ")
    return [t.strip() for t in tokens]


"""
    Prints list of available commands of the software.
"""


def help():
    # prints the choices for commands and parameters
    print("\n*** Please enter one of the following commands ***")
    print("> help")
    print("> sign_up <email> <password> <first_name> <last_name> <plan_id>")
    print("> sign_in <email> <password>")
    print("> sign_out")
    print("> show_plans")
    print("> show_subscription")
    print("> subscribe <plan_id>")
    print("> watched_movies <movie_id_1> <movie_id_2> <movie_id_3> ... <movie_id_n>")
    print("> search_for_movies <keyword_1> <keyword_2> <keyword_3> ... <keyword_n>")
    print("> suggest_movies")
    print("> quit")


"""
    Saves customer with given details.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
"""


def sign_up(conn, email, password, first_name, last_name, plan_id):		
    try:
        cur = conn.cursor()
        flag = False
        cur.execute("""SELECT email FROM customer""")
        result_set = cur.fetchall()
        for result in result_set:
            if result==email:
                flag = True
                break
        if flag==False:
            sql = """INSERT INTO Customer (email, password, first_name, last_name, session_count, plan_id) VALUES (%s, %s, %s, %s, %s, %s)"""
            val = (email, password, first_name, last_name, 0, plan_id,)
            cur.execute(sql, val)
            conn.commit()
            return True, CMD_EXECUTION_SUCCESS
        else:
            conn.rollback()
    except (Exception, psycopg2.DatabaseError) as e:
        conn.rollback()
        return False, CMD_EXECUTION_FAILED

"""
    Retrieves customer information if email and password is correct and customer's session_count < max_parallel_sessions.
    - Return type is a tuple, 1st element is a customer object and 2nd element is the response message from messages.py.
    - If email or password is wrong, return tuple (None, USER_SIGNIN_FAILED).
    - If session_count < max_parallel_sessions, commit changes (increment session_count) and return tuple (customer, CMD_EXECUTION_SUCCESS).
    - If session_count >= max_parallel_sessions, return tuple (None, USER_ALL_SESSIONS_ARE_USED).
    - If any exception occurs; rollback, do nothing on the database and return tuple (None, USER_SIGNIN_FAILED).
"""


def sign_in(conn, email, password):
    try:
        cur = conn.cursor()
        sql = """SELECT email, password 
                 FROM Customer 
                 WHERE email = %s AND password = %s;"""
        val = (email, password,)
        cur.execute(sql, val)
        if cur.fetchone() is None:
            return None, USER_SIGNIN_FAILED
        sql = """SELECT customer_id 
                 FROM Customer c, Plan p 
                 WHERE email = %s AND password = %s AND session_count < max_parallel_sessions AND 
                       c.plan_id = p.plan_id;"""
        val = (email, password,)
        cur.execute(sql, val)
        if cur.fetchone() is None:
            return None, USER_ALL_SESSIONS_ARE_USED
        else:
            sql = """SELECT session_count 
                     FROM Customer 
                     WHERE email = %s AND password = %s;"""
            val = (email, password,)
            cur.execute(sql, val)
            record = cur.fetchone()[0]
            new_session_count = int(record)
            new_session_count += 1
            sql = """UPDATE Customer SET session_count = %s WHERE email = %s AND password = %s;"""
            val = (new_session_count, email, password,)
            cur.execute(sql, val)
            sql = """SELECT * 
                     FROM Customer 
                     WHERE email = %s AND password = %s;"""
            val = (email, password,)
            cur.execute(sql, val)
            result = cur.fetchall()
            customer = Customer(result[0][0], result[0][1], result[0][3], result[0][4], result[0][5], result[0][6])
            conn.commit()
            return customer, CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError) as e:
        conn.rollback()
        return None, USER_SIGNIN_FAILED


"""
    Signs out from given customer's account.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - Decrement session_count of the customer in the database.
    - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
"""


def sign_out(conn, customer):
    try:
        cur = conn.cursor()
        sql = """SELECT session_count 
                 FROM Customer 
                 WHERE customer_id = %s;"""
        val = (customer.customer_id,)
        cur.execute(sql, val)
        record = cur.fetchone()[0]
        new_session_count = int(record)
        if new_session_count>0:
            new_session_count -= 1
        sql = """UPDATE Customer SET session_count = %s WHERE customer_id = %s;"""
        val = (new_session_count, customer.customer_id,)
        cur.execute(sql, val)
        conn.commit()
        return True, CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError) as e:
        conn.rollback()
        return False, CMD_EXECUTION_FAILED
    


"""
    Quits from program.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - Remember to sign authenticated user out first.
    - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
"""


def quit(conn, customer):
    try:
        if customer:
            return False, CMD_EXECUTION_FAILED
        else:
            return True, CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError) as e:
        return False, CMD_EXECUTION_FAILED
    


"""
    Retrieves all available plans and prints them.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - If the operation is successful; print available plans and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).

    Output should be like:
    #|Name|Resolution|Max Sessions|Monthly Fee
    1|Basic|720P|2|30
    2|Advanced|1080P|4|50
    3|Premium|4K|10|90
"""


def show_plans(conn):
    try:
        cur = conn.cursor()
        print("#|Name|Resolution|Max Sessions|Monthly Fee")
        cur.execute("""SELECT * FROM Plan;""")
        result = cur.fetchall()
        n = cur.rowcount
        for x in range(n):
            print("{}|{}|{}|{}|{}".format(result[x][0], result[x][1], result[x][2], result[x][3], result[x][4]))
        conn.commit()
        return True, CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError) as e:
        conn.rollback()
        return False, CMD_EXECUTION_FAILED


"""
    Retrieves authenticated user's plan and prints it. 
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - If the operation is successful; print the authenticated customer's plan and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).

    Output should be like:
    #|Name|Resolution|Max Sessions|Monthly Fee
    1|Basic|720P|2|30
"""


def show_subscription(conn, customer):
    try:
        cur = conn.cursor()
        print("#|Name|Resolution|Max Sessions|Monthly Fee")
        sql = """SELECT * 
                 FROM Plan 
                 WHERE plan_id = %s;"""
        val = (customer.plan_id,)
        cur.execute(sql, val)
        result = cur.fetchall()
        for x in range(1):
            print("{}|{}|{}|{}|{}".format(result[x][0], result[x][1], result[x][2], result[x][3], result[x][4]))
        conn.commit()
        return True, CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError) as e:
        conn.rollback()
        return False, CMD_EXECUTION_FAILED


"""
    Insert customer-movie relationships to Watched table if not exists in Watched table.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - If a customer-movie relationship already exists, do nothing on the database and return (True, CMD_EXECUTION_SUCCESS).
    - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any one of the movie ids is incorrect; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
"""


def watched_movies(conn, customer, movie_ids):
    try:
        cur = conn.cursor()
        liste = [] 
        flag = False
        for x in movie_ids: 
            removed_quote = x.strip('\"')
            sql = """SELECT customer_id 
                     FROM Watched 
                     WHERE customer_id = %s AND movie_id = %s;"""
            val = (customer.customer_id, removed_quote,)
            cur.execute(sql, val)
            if cur.fetchone() is None:
                sql = """SELECT * 
                         FROM Movies 
                         WHERE movie_id = %s;"""
                val = (removed_quote,)
                cur.execute(sql, val)
                if cur.fetchone() is None:
                    flag = True
                    break
                else:
                    liste.append(x)
        if flag == False:
            for x in liste:
                removed_quote = x.strip('\"')
                sql = """INSERT INTO Watched (customer_id, movie_id) VALUES (%s, %s)"""
                val = (customer.customer_id, removed_quote,)
                cur.execute(sql, val)
            conn.commit()
            return True, CMD_EXECUTION_SUCCESS
        else:
            conn.rollback()
            return False, CMD_EXECUTION_FAILED
    except (Exception, psycopg2.DatabaseError) as e:
        conn.rollback()
        return False, CMD_EXECUTION_FAILED


"""
    Subscribe authenticated customer to new plan.
    - Return type is a tuple, 1st element is a customer object and 2nd element is the response message from messages.py.
    - If target plan does not exist on the database, return tuple (None, SUBSCRIBE_PLAN_NOT_FOUND).
    - If the new plan's max_parallel_sessions < current plan's max_parallel_sessions, return tuple (None, SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE).
    - If the operation is successful, commit changes and return tuple (customer, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; rollback, do nothing on the database and return tuple (None, CMD_EXECUTION_FAILED).
"""


def subscribe(conn, customer, plan_id):
    try:
        cur = conn.cursor()
        sql = """SELECT plan_id 
                 FROM Plan 
                 WHERE plan_id = %s;"""
        val = (plan_id,)
        cur.execute(sql, val)
        if cur.fetchone() is None:
            return None, SUBSCRIBE_PLAN_NOT_FOUND
        sql = """SELECT max_parallel_sessions 
                 FROM Plan 
                 WHERE plan_id = %s;"""
        val = (plan_id,)
        cur.execute(sql, val)
        record = cur.fetchone()[0]
        new_max_parallel_sessions = int(record)
        sql = """SELECT max_parallel_sessions 
                 FROM Plan 
                 WHERE plan_id = %s;"""
        val = (customer.plan_id,)
        cur.execute(sql, val)
        record = cur.fetchone()[0]
        current_max_parallel_sessions = int(record)
        if new_max_parallel_sessions < current_max_parallel_sessions:
            return None, SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE
        else:
            sql = """UPDATE Customer SET plan_id = %s WHERE customer_id = %s;"""
            val = (plan_id, customer.customer_id,)
            cur.execute(sql, val)
            customer.plan_id = plan_id
            conn.commit()
            return customer, CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError) as e:
        conn.rollback()
        return False, CMD_EXECUTION_FAILED
"""
    Searches for movies with given search_text.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - Print all movies whose titles contain given search_text IN CASE-INSENSITIVE MANNER.
    - If the operation is successful; print movies found and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).

    Output should be like:
    Id|Title|Year|Rating|Votes|Watched
    "tt0147505"|"Sinbad: The Battle of the Dark Knights"|1998|2.2|149|0
    "tt0468569"|"The Dark Knight"|2008|9.0|2021237|1
    "tt1345836"|"The Dark Knight Rises"|2012|8.4|1362116|0
    "tt3153806"|"Masterpiece: Frank Millers The Dark Knight Returns"|2013|7.8|28|0
    "tt4430982"|"Batman: The Dark Knight Beyond"|0|0.0|0|0
    "tt4494606"|"The Dark Knight: Not So Serious"|2009|0.0|0|0
    "tt4498364"|"The Dark Knight: Knightfall - Part One"|2014|0.0|0|0
    "tt4504426"|"The Dark Knight: Knightfall - Part Two"|2014|0.0|0|0
    "tt4504908"|"The Dark Knight: Knightfall - Part Three"|2014|0.0|0|0
    "tt4653714"|"The Dark Knight Falls"|2015|5.4|8|0
    "tt6274696"|"The Dark Knight Returns: An Epic Fan Film"|2016|6.7|38|0
"""


def search_for_movies(conn, customer, search_text):
    try:
        cur = conn.cursor()
        upper = search_text.upper()
        sql = """SELECT * 
                 FROM Movies 
                 WHERE UPPER(title) LIKE %s;"""
        val = ('%' + upper + '%', )
        cur.execute(sql, val)
        if cur.fetchone() is not None:
            sql = """SELECT * 
                     FROM Movies 
                     WHERE UPPER(title) LIKE %s 
                     ORDER BY movie_id ASC;"""
            val = ('%' + upper + '%', )
            cur.execute(sql, val)
            result = cur.fetchall()
            n = cur.rowcount
            print("Id|Title|Year|Rating|Votes|Watched")
            for x in range(n):
                sql = """SELECT COUNT(*) 
                         FROM Watched 
                         WHERE movie_id = %s AND customer_id = %s;"""
                val = (result[x][0], customer.customer_id)
                cur.execute(sql, val)
                record = cur.fetchone()[0]
                print("{}|{}|{}|{}|{}|{}".format("\"" + result[x][0] + "\"", "\"" + result[x][1] + "\"", result[x][2], result[x][3], result[x][4], record))
            conn.commit()
            return True, CMD_EXECUTION_SUCCESS 
        else:
            return False, CMD_EXECUTION_FAILED
    except (Exception, psycopg2.DatabaseError) as e:
        conn.rollback()
        return False, CMD_EXECUTION_FAILED


"""
    Suggests combination of these movies:
        1- Find customer's genres. For each genre, find movies with most number of votes among the movies that the customer didn't watch.

        2- Find top 10 movies with most number of votes and highest rating, such that these movies are released 
           after 2010 ( [2010, today) ) and the customer didn't watch these movies.
           (descending order for votes, descending order for rating)

        3- Find top 10 movies with votes higher than the average number of votes of movies that the customer watched.
           Disregard the movies that the customer didn't watch.
           (descending order for votes)

    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.    
    - Output format and return format are same with search_for_movies.
    - Order these movies by their movie id, in ascending order at the end.
    - If the operation is successful; print movies suggested and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
"""


def suggest_movies(conn, customer):
    try:
        cur = conn.cursor()
        liste = []
        sql = """SELECT DISTINCT genre_name 
                 FROM genres g, watched w 
                 WHERE g.movie_id = w.movie_id AND w.customer_id = %s;"""
        val = (customer.customer_id,)
        cur.execute(sql, val)
        genres = cur.fetchall()
        for genre in genres:
            sql = """SELECT m.movie_id, m.votes 
                     FROM movies m, genres g where m.movie_id = g.movie_id AND g.genre_name = %s AND 
                          m.movie_id NOT IN (SELECT movie_id 
                                             FROM watched 
                                             WHERE customer_id= %s)
                     GROUP BY m.movie_id, m.votes 
                     HAVING m.votes >= ALL (SELECT m1.votes 
                                            FROM movies m1, genres g1 
                                            WHERE m1.movie_id = g1.movie_id AND g1.genre_name = %s AND 
                                                  m1.movie_id NOT IN (SELECT movie_id 
                                                                      FROM watched 
                                                                      WHERE customer_id = %s));;"""
            val = (genre, customer.customer_id, genre, customer.customer_id,)
            cur.execute(sql, val)
            result = cur.fetchall()
            liste.append(result[0][0])
        sql = """SELECT * 
                 FROM movies 
                 WHERE movie_year >= 2010 AND movie_id NOT IN (SELECT movie_id 
                                                               FROM watched 
                                                               WHERE customer_id = %s) 
                 ORDER BY votes DESC, rating DESC 
                 LIMIT 10;"""
        val = (customer.customer_id,)
        cur.execute(sql, val)
        high_vals = cur.fetchall()
        for high_val in high_vals:
            liste.append(high_val[0])
        sql = """SELECT * 
                 FROM movies 
                 WHERE movie_id NOT IN (SELECT movie_id 
                                        FROM watched 
                                        WHERE customer_id = %s)
                 GROUP BY movie_id
                 HAVING votes > (SELECT AVG(votes)
                                 FROM movies m, watched w 
                                 WHERE m.movie_id = w.movie_id AND w.customer_id = %s)
                 ORDER BY votes DESC, rating DESC LIMIT 10;"""
        val = (customer.customer_id, customer.customer_id)
        cur.execute(sql, val)
        avg_vals = cur.fetchall()
        for avg_val in avg_vals:
            liste.append(avg_val[0])
        liste = list(dict.fromkeys(liste))
        liste = sorted(liste)
        print("Id|Title|Year|Rating|Votes")
        for value in liste:
            sql = """SELECT * 
                     FROM movies 
                     WHERE movie_id = %s;"""
            val = (value,)
            cur.execute(sql, val)
            result = cur.fetchall()
            print("{}|{}|{}|{}|{}".format("\"" + result[0][0] + "\"", "\"" + result[0][1] + "\"", result[0][2], result[0][3], result[0][4]))
        return True, CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError) as e:
        conn.rollback()
        return False, CMD_EXECUTION_FAILED
