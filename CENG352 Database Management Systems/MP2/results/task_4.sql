CREATE TABLE ActiveAuthors (
	name varchar(255) 
);



INSERT INTO ActiveAuthors(name)
SELECT DISTINCT a.name
FROM author a, field f, publication p
WHERE f.field_name = 'author' AND f.field_value = a.name AND p.pub_key = f.pub_key AND 
     (p.year = 2018 OR p.year = 2019 OR p.year = 2020)  
ORDER BY a.name



CREATE OR REPLACE FUNCTION activeauthor() RETURNS TRIGGER AS $activeauthor$
	BEGIN
   		IF (SELECT COUNT(p.pub_id) 
		    FROM publication p 
		    WHERE p.pub_id = NEW.pub_id AND 
			  (p.year = 2018 OR p.year = 2019 OR p.year = 2020))=1 THEN
       		INSERT INTO activeauthors(name)
		SELECT a.name
		FROM author a
		WHERE NEW.author_id = a.author_id AND 
		      a.name NOT IN (SELECT name FROM activeauthors);
   		END IF;
   		RETURN NEW;
   	END;
$activeauthor$ LANGUAGE 'plpgsql';



CREATE TRIGGER activeauthor
AFTER INSERT ON authored
FOR EACH ROW EXECUTE PROCEDURE activeauthor();
