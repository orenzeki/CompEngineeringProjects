SELECT a.name AS author_name, count(*) AS pub_count
FROM author a, publication p, field f
WHERE p.pub_key = f.pub_key AND f.field_name = 'author' AND a.name = f.field_value
GROUP BY a.name
HAVING COUNT(*) >= 150 AND count(*) < 200
ORDER BY pub_count,author_name ASC ;
