/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ceng.ceng351.musicdb;


import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
/**
 *
 * @author zeki
 */
class MUSICDB {
    
    Connection con = null;
    Statement stat = null;
    ResultSet rs = null;
    
    void initialize() {        
        try{  
        Class.forName("com.mysql.cj.jdbc.Driver");  
        con=DriverManager.getConnection("jdbc:mysql://144.122.71.57:8084/db2264612","e2264612","48338f9c");  
        
        
    }   catch (ClassNotFoundException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
    
    
        
    }

    int dropTables() {
        int counter = 0;
        try {
            stat = con.createStatement();
            String sql4 = "DROP TABLE IF EXISTS LISTEN ";
            stat.executeUpdate(sql4);
            counter++;
            
            String sql3 = "DROP TABLE IF EXISTS SONG ";
            stat.executeUpdate(sql3);
            counter++;
          
            String sql2 = "DROP TABLE IF EXISTS ALBUM ";
            stat.executeUpdate(sql2);
            counter++;
         
            String sql1 = "DROP TABLE IF EXISTS ARTIST ";
            stat.executeUpdate(sql1);
            counter++;
      
            System.out.println("Dropping Tables");
            String sql = "DROP TABLE IF EXISTS USER ";
            stat.executeUpdate(sql);
            counter++;
           
            System.out.println("Dropped Tables");
            stat.close();
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
      
      return counter;
        
    }

    int createTables() {
        int counter = 0;
        try {
           
            stat = con.createStatement();
            System.out.println("Creating Tables");
            
            String sql = "CREATE TABLE USER " +
                   "(userID INTEGER , " +
                   " userName VARCHAR(60), " + 
                   " email VARCHAR(30), " + 
                   " password VARCHAR(30), " + 
                   " PRIMARY KEY ( userID ))"; 

            stat.executeUpdate(sql);
            counter++;
            
            
            String sql1 = "CREATE TABLE ARTIST " +
                   "(artistID INTEGER , " +
                   " artistName VARCHAR(60), " + 
                   " PRIMARY KEY ( artistID ))"; 

            stat.executeUpdate(sql1);
            counter++;
            
            
            String sql2 = "CREATE TABLE ALBUM " +
                   "(albumID INTEGER , " +
                   " title VARCHAR(60), " + 
                   " albumGenre VARCHAR(30), " + 
                   " albumRating DOUBLE, " +
                   " releaseDate DATE, " + 
                   " artistID INTEGER, " + 
                   " PRIMARY KEY ( albumID ), " +
                   " FOREIGN KEY (artistID) REFERENCES ARTIST(artistID))"; 

            stat.executeUpdate(sql2);
            counter++;
            
            
            String sql3 = "CREATE TABLE SONG " +
                   "(songID INTEGER, " +
                   " songName VARCHAR(60), " + 
                   " genre VARCHAR(30), " + 
                   " rating DOUBLE, " +
                   " artistID INTEGER, " + 
                   " albumID INTEGER, " +
                   " PRIMARY KEY ( songID )," +
                   " FOREIGN KEY (artistID) REFERENCES ARTIST(artistID)," +
                   " FOREIGN KEY (albumID) REFERENCES ALBUM(albumID))"; 

            stat.executeUpdate(sql3);
            counter++;
            
            
            String sql4 = "CREATE TABLE LISTEN " +
                   "(userID INTEGER, " +
                   " songID INTEGER, " + 
                   " lastListenTime TIMESTAMP, " + 
                   " listenCount INTEGER, " +                 
                   " PRIMARY KEY ( userID,songID )," +
                   " FOREIGN KEY (userID) REFERENCES USER(userID)," +
                   " FOREIGN KEY (songID) REFERENCES SONG(songID) ON DELETE CASCADE ON UPDATE CASCADE)"; 

            stat.executeUpdate(sql4);
            counter++;
            
            System.out.println("Created Tables");
            stat.close();
            
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        return counter;
    }

    int insertUser(User[] users) {
        int i=0;
        String sql;
        try {
            stat = con.createStatement();
            System.out.println("Inserting records of USER");
            
            for(i=0; i< users.length; i++ ){
                   sql = "INSERT INTO USER " +
                   "VALUES (" + users[i].getUserID() + ", '" + users[i].getUserName() + "', '" 
                              + users[i].getEmail() + "', '" + users[i].getPassword() + "')";
                   stat.executeUpdate(sql);
            }
            
            stat.close();
            
            
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return i;
    }

    int insertArtist(Artist[] artists) {
         int i=0;
         String sql;
        try {
            stat = con.createStatement();
            System.out.println("Inserting records of ARTIST");
            
            for(i=0; i< artists.length; i++ ){
                   sql = "INSERT INTO ARTIST " +
                   "VALUES (" + artists[i].getArtistID() + ", '" + artists[i].getArtistName() + "')"; 
                   stat.executeUpdate(sql);          
            }
            
            stat.close();
            
            
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return i;
    }

    int insertAlbum(Album[] albums) {
        int i=0;
        String sql;
        try {
            System.out.println("Inserting records of ALBUM");
            stat = con.createStatement();
            
            for(i=0; i< albums.length; i++ ){
                   sql = "INSERT INTO ALBUM " +
                   "VALUES (" + albums[i].getAlbumID() + ", \"" + albums[i].getTitle() + "\", '" 
                              + albums[i].getAlbumGenre() + "', " + albums[i].getAlbumRating() + 
                              ", '" + albums[i].getReleaseDate() + "', " + albums[i].getArtistID() + ")";
                   stat.executeUpdate(sql);
            }
            stat.close();
            
            
            
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return i;
    }

    int insertSong(Song[] songs) {
         int i=0;
         String sql;
        try {
            System.out.println("Inserting records of SONG");
            stat = con.createStatement();
            
            for(i=0; i< songs.length; i++ ){
                   sql = "INSERT INTO SONG " +
                   "VALUES (" + songs[i].getSongID() + ", '" + songs[i].getSongName() + "', '" 
                              + songs[i].getGenre() + "', " + songs[i].getRating() + 
                              ", " + songs[i].getArtistID() + ", " + songs[i].getAlbumID() +")";
                   stat.executeUpdate(sql);
            }
            stat.close();
            
            
            
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return i;
    }

    int insertListen(Listen[] listens) {
        int i=0;
        String sql;
        try {
            System.out.println("Inserting records of LISTEN");
            stat = con.createStatement();
            
            for(i=0; i< listens.length; i++ ){
                   sql = "INSERT INTO LISTEN " +
                   "VALUES (" + listens[i].getUserID() + ", " + listens[i].getSongID() + ", '" 
                              + listens[i].getLastListenTime() + "', " + listens[i].getListenCount() + ")";
                    stat.executeUpdate(sql);
            }
            stat.close();
            
            
            
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return i;
    }

    QueryResult.ArtistNameSongNameGenreRatingResult[] getHighestRatedSongs() {
        ArrayList<QueryResult.ArtistNameSongNameGenreRatingResult> array = new ArrayList<>();
        try {
            stat = con.createStatement();
            System.out.println("Query getHighestRatedSongs");
            String sql = "SELECT DISTINCT A.artistName, S.songName, S.genre, S.rating " +
                         "FROM SONG S, ARTIST A " +
                         "WHERE S.artistID = A.artistID AND S.rating = (SELECT MAX(S1.rating) " +
                                           "FROM SONG S1) " +
                         "ORDER BY A.artistName";
            rs = stat.executeQuery(sql);
            while (rs.next())
            {
            
            String artistName = rs.getString("artistName");
            String songName = rs.getString("songName");
            String genre = rs.getString("genre");
            double rating = rs.getDouble("rating");
            
            array.add(new QueryResult.ArtistNameSongNameGenreRatingResult(artistName, songName, genre, rating));
            }
            stat.close();
            
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return (QueryResult.ArtistNameSongNameGenreRatingResult[]) array.toArray(new  QueryResult.ArtistNameSongNameGenreRatingResult[array.size()]);
    }

    QueryResult.TitleReleaseDateRatingResult getMostRecentAlbum(String artistName) {
        QueryResult.TitleReleaseDateRatingResult result = null;
        try {
            stat = con.createStatement();
            System.out.println("Query getMostRecentAlbum");
            String sql = "SELECT DISTINCT A.title, A.releaseDate, A.albumRating " +
                         "FROM ALBUM A, ARTIST B " +
                         "WHERE A.artistID = B.artistID AND B.artistName = '" +
                          artistName + "' AND A.releaseDate = (SELECT MAX(A1.releaseDate) " +
                                                              "FROM ALBUM A1, ARTIST B1 " +
                                                              "WHERE A1.artistID = B1.artistID " +
                                                              "AND B1.artistName = '" +
                                                               artistName + "') " +
                         "ORDER BY A.title";
            rs = stat.executeQuery(sql);
            while (rs.next())
            {
            
            String title = rs.getString("title");
            String releaseDate = rs.getString("releaseDate");
            double albumRating = rs.getDouble("albumRating");
            
            result = new QueryResult.TitleReleaseDateRatingResult(title, releaseDate, albumRating);
            }
            stat.close();
            
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return result;
    
    }

    QueryResult.ArtistNameSongNameGenreRatingResult[] getCommonSongs(String userName1, String userName2) throws SQLException {
        ArrayList<QueryResult.ArtistNameSongNameGenreRatingResult> array = new ArrayList<>();
        try {
            stat = con.createStatement();
            System.out.println("Query getCommonSongs");
            String sql = "SELECT DISTINCT A.artistName, S.songName, S.genre, S.rating " +
                         "FROM SONG S, LISTEN L, USER U, ARTIST A " +
                         "WHERE S.songID = L.songID AND U.userID = L.userID AND U.userName = '" + 
                          userName1 + "' " +
                         "AND A.artistID = S.artistID AND S.songID IN " +
                         "(SELECT S1.songID " +
                         "FROM SONG S1, LISTEN L1, USER U1, ARTIST A1 " +
                         "WHERE S1.songID = L1.songID AND U1.userID = L1.userID AND U1.userName = '" + 
                          userName2 + "' " +
                         "AND A1.artistID = S1.artistID) " +
                         "ORDER BY S.rating";
            rs = stat.executeQuery(sql);
            while (rs.next())
            {
            
            String artistName = rs.getString("artistName");
            String songName = rs.getString("songName");
            String genre = rs.getString("genre");
            double rating = rs.getDouble("rating");
            
            array.add(new QueryResult.ArtistNameSongNameGenreRatingResult(artistName, songName, genre, rating));
            }
            stat.close();
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return (QueryResult.ArtistNameSongNameGenreRatingResult[]) array.toArray(new  QueryResult.ArtistNameSongNameGenreRatingResult[array.size()]);
    }

    QueryResult.ArtistNameNumberOfSongsResult[] getNumberOfTimesSongsListenedByUser(String userName3) {
        ArrayList<QueryResult.ArtistNameNumberOfSongsResult> array = new ArrayList<>();
        try {
            stat = con.createStatement();
            System.out.println("Query getNumberOfTimesSongsListenedByUser");
            String sql = "SELECT DISTINCT A.artistName, SUM(L.listenCount) AS listenCount " +
                         "FROM ARTIST A, SONG S, LISTEN L, USER U " +
                         "WHERE A.artistID = S.artistID AND S.songID = L.songID AND U.userID = L.userID " +
                         "AND U.userName = '" + userName3 + "' " +
                         "GROUP BY A.artistName " +
                         "ORDER BY A.artistName";
            rs = stat.executeQuery(sql);
            while (rs.next())
            {
            
            String artistName = rs.getString("artistName");
            int listenCount = rs.getInt("listenCount");
            
            
            array.add(new QueryResult.ArtistNameNumberOfSongsResult(artistName, listenCount));
            }
            stat.close();
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return (QueryResult.ArtistNameNumberOfSongsResult[]) array.toArray(new  QueryResult.ArtistNameNumberOfSongsResult[array.size()]);
    }

    User[] getUsersWhoListenedAllSongs(String artistName) {
        ArrayList<User> array = new ArrayList<>();
        try {
            stat = con.createStatement();
            System.out.println("Query getUsersWhoListenedAllSongs");
            String sql = "SELECT DISTINCT U.userID, U.userName, U.email, U.password " +
                         "FROM USER U " +
                         "WHERE NOT EXISTS (SELECT S.songID " +
                                           "FROM SONG S, ARTIST A " +
                                           "WHERE A.artistID = S.artistID AND A.artistName = '" +
                                            artistName + "' " +
                                           "AND NOT EXISTS " +
                                           "(SELECT L.songID " +
                                           "FROM LISTEN L " +
                                           "WHERE L.userID = U.userID AND L.songID = S.songID)) " +
                         "ORDER BY userID";
            rs = stat.executeQuery(sql);
            while (rs.next())
            {
            int userID = rs.getInt("userID");
            String userName = rs.getString("userName");
            String email = rs.getString("email");
            String password = rs.getString("password");
            
            
            array.add(new User(userID, userName, email, password));
            }
            stat.close();
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return (User[]) array.toArray(new  User[array.size()]);

    }

    QueryResult.UserIdUserNameNumberOfSongsResult[] getUserIDUserNameNumberOfSongsNotListenedByAnyone() {
        ArrayList<QueryResult.UserIdUserNameNumberOfSongsResult> array = new ArrayList<>();
        try {
            stat = con.createStatement();
            System.out.println("Query getUserIDUserNameNumberOfSongsNotListenedByAnyone");
            String sql = "SELECT DISTINCT U.userID, U.userName, COUNT(*) AS scount " +
                         "FROM USER U, LISTEN L, SONG S " +
                         "WHERE U.userID = L.userID AND L.songID = S.songID AND S.songID NOT IN " +
                                           "(SELECT S1.songID " +
                                           "FROM SONG S1, USER U1, LISTEN L1 " +
                                           "WHERE U1.userID = L1.userID AND L1.songID = S1.songID " +
                                           "AND U.userID <> U1.userID) " +
                         "GROUP BY userID " +
                         "ORDER BY userID";
            rs = stat.executeQuery(sql);
            while (rs.next())
            {
            int userID = rs.getInt("userID");
            String userName = rs.getString("userName");
            int scount = rs.getInt("scount");
            
            
            
            array.add(new QueryResult.UserIdUserNameNumberOfSongsResult(userID, userName, scount));
            }
            stat.close();
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return (QueryResult.UserIdUserNameNumberOfSongsResult[]) array.toArray(new  QueryResult.UserIdUserNameNumberOfSongsResult[array.size()]);
    }

    Artist[] getArtistSingingPopGreaterAverageRating(double avgRating) {
          ArrayList<Artist> array = new ArrayList<>();
        try {
            stat = con.createStatement();
            System.out.println("Query getArtistSingingPopGreaterAverageRating");
            String sql = "SELECT DISTINCT A.artistID, A.artistName " +
                         "FROM ARTIST A, SONG S " +
                         "WHERE  A.artistID = S.artistID AND S.genre = 'Pop' AND '" +
                          avgRating + "' < (SELECT AVG(S1.rating) " +
                                            "FROM ARTIST A1, SONG S1 " +
                                            "WHERE A1.artistID = S1.artistID AND S1.genre = 'Pop') " +
                         "ORDER BY artistID";
            rs = stat.executeQuery(sql);
            while (rs.next())
            {
            int artistID = rs.getInt("artistID");
            String artistName = rs.getString("artistName");
            
            
            
            array.add(new Artist(artistID, artistName));
            }
            stat.close();
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return (Artist[]) array.toArray(new  Artist[array.size()]);
    
    }

    Song[] retrieveLowestRatedAndLeastNumberOfListenedSongs() {
        ArrayList<Song> array = new ArrayList<>();
        try {
            stat = con.createStatement();
            System.out.println("Query retrieveLowestRatedAndLeastNumberOfListenedSong");
            String sql = "SELECT DISTINCT S.songID, S.songName, S.genre, S.rating , S.artistID, S.albumID " +
                         "FROM LISTEN L1, SONG S " +
                         "WHERE S.songID = L1.songID AND S.genre = 'Pop' " +
                         "GROUP BY S.songID " +
                         "HAVING SUM(L1.listenCount) = " +
                               "(SELECT MIN(Temp.listen) " +
                                "FROM (SELECT  SUM(L.listenCount) AS listen " +
                                      "FROM LISTEN L " +
                                      "WHERE L.songID IN (SELECT S1.songID " +
                                                         "FROM SONG S1 " +
                                                         "WHERE S1.genre = 'Pop' AND S1.rating = (SELECT MIN(S2.rating) " +
                                                                                                 "FROM SONG S2 " +
                                                                                                 "WHERE S2.genre = 'Pop')) " +
                         "GROUP BY L.songID) AS Temp)";
                                   
            rs = stat.executeQuery(sql);
            while (rs.next())
            {
            int songID = rs.getInt("songID");
            String songName = rs.getString("songName");
            String genre = rs.getString("genre");
            double rating = rs.getDouble("rating");
            int artistID = rs.getInt("artistID");
            int albumID = rs.getInt("albumID");
            
            array.add(new Song(songID, songName, genre, rating, artistID, albumID));
            }
            stat.close();
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return (Song[]) array.toArray(new  Song[array.size()]);
    
    }

    int multiplyRatingOfAlbum(String releaseDate) {
        int multiply = 0;
        try {
            stat = con.createStatement();
            System.out.println("multiplyRatingOfAlbum");
            String sql = "UPDATE ALBUM A " +
                         "SET A.albumRating = A.albumRating*1.5 " + 
                         "WHERE A.releaseDate > '" +releaseDate + "'";
            stat.executeUpdate(sql);
            multiply++;
            stat.close();
            
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return multiply;
    }

    Song deleteSong(String songName) {
        Song result = null;
        try {
            stat = con.createStatement();
            System.out.println("deleteSong");
            
            String sql1 = "SELECT S.songID, S.songName, S.genre, S.rating, S.artistID, S.albumID " +
                          "FROM SONG S " +
                          "WHERE S.songName = '" +
                          songName + "'";
            rs = stat.executeQuery(sql1);
            while (rs.next())
            {
            int songID =rs.getInt("songID");
            String songname = rs.getString("songName");
            double rating = rs.getDouble("rating");
            String genre = rs.getString("genre");
            int artistID =rs.getInt("artistID");
            int albumID =rs.getInt("albumID");
            
            result = new Song(songID, songname, genre, rating, artistID, albumID);
            }
            stat = con.createStatement();
            String sql = "DELETE " +
                         "FROM SONG " +
                         "WHERE songName = \"" +
                          songName + "\"";
            stat.executeUpdate(sql);
            
        } catch (SQLException ex) {
            Logger.getLogger(MUSICDB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return result;
    }

   
    
}
