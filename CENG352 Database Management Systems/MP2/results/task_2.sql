INSERT INTO author (name)
SELECT DISTINCT f.field_value
FROM field f
WHERE f.field_name = 'author';



INSERT INTO publication (pub_key, title, year)
SELECT DISTINCT f.pub_key, f.field_value, CAST(f2.field_value  AS INT)
FROM field f
LEFT JOIN field f2 ON f.pub_key = f2.pub_key
WHERE f.field_name = 'title' AND f2.field_name = 'year';



INSERT INTO article (pub_id, journal, month, volume, number)
SELECT DISTINCT p.pub_id, f.field_value, f2.field_value, f3.field_value, f4.field_value
FROM publication p
LEFT JOIN field f ON p.pub_key = f.pub_key AND f.field_name = 'journal'
LEFT JOIN field f2 ON p.pub_key = f2.pub_key AND f2.field_name = 'month'
LEFT JOIN field f3 ON p.pub_key = f3.pub_key AND f3.field_name = 'volume'
LEFT JOIN field f4 ON p.pub_key = f4.pub_key AND f4.field_name = 'number'
WHERE p.pub_key IN (SELECT p2.pub_key
		    FROM pub p2
		    WHERE p2.pub_type = 'article');



INSERT INTO book (pub_id, publisher, isbn)
SELECT DISTINCT p.pub_id, f.field_value, MAX(f2.field_value)
FROM publication p
LEFT JOIN field f ON p.pub_key = f.pub_key AND f.field_name = 'publisher'
LEFT JOIN field f2 ON p.pub_key = f2.pub_key AND f2.field_name = 'isbn'
WHERE p.pub_key IN (SELECT p2.pub_key
		    FROM pub p2
		    WHERE p2.pub_type = 'book')
GROUP BY p.pub_id, f.field_value;



INSERT INTO incollection (pub_id, book_title, publisher, isbn)
SELECT DISTINCT p.pub_id, f.field_value, f2.field_value, MAX(f3.field_value)
FROM publication p
LEFT JOIN field f ON p.pub_key = f.pub_key AND f.field_name = 'book_title'
LEFT JOIN field f2 ON p.pub_key = f2.pub_key AND f2.field_name = 'publisher'
LEFT JOIN field f3 ON p.pub_key = f3.pub_key AND f3.field_name = 'isbn'
WHERE p.pub_key IN (SELECT p2.pub_key
		    FROM pub p2
		    WHERE p2.pub_type = 'incollection')
GROUP BY p.pub_id, f.field_value, f2.field_value;



INSERT INTO inproceedings (pub_id, book_title, editor)
SELECT DISTINCT p.pub_id, f.field_value, f2.field_value 
FROM publication p
LEFT JOIN field f ON p.pub_key = f.pub_key AND f.field_name = 'book_title'
LEFT JOIN field f2 ON p.pub_key = f2.pub_key AND f2.field_name = 'editor'
WHERE p.pub_key IN (SELECT p2.pub_key
		    FROM pub p2
		    WHERE p2.pub_type = 'inproceedings');


				   
INSERT INTO authored (author_id, pub_id)
SELECT a.author_id, p.pub_id 
FROM author a, publication p, field f
WHERE p.pub_key = f.pub_key AND f.field_name = 'author' AND f.field_value = a."name";

