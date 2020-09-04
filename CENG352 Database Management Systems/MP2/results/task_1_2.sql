SELECT a.field_name
FROM (SELECT p.pub_type, f.field_name
      FROM pub p, field f
      WHERE p.pub_key = f.pub_key AND 
            p.pub_type IN ('article', 'proceedings', 'incollection', 'inproceedings', 'www', 'book', 'mastersthesis')
      GROUP BY p.pub_type, f.field_name) AS a
GROUP BY a.field_name
HAVING COUNT(a.field_name) >= 7
ORDER BY a.field_name;
