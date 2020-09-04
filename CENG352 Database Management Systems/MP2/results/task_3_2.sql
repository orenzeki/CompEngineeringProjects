SELECT a.name AS author_name, COUNT(*) AS pub_count
FROM author a, publication p, field f, field f2
WHERE p.pub_key = f.pub_key AND f.field_name = 'author' AND a.name = f.field_value AND 
      p.pub_key = f2.pub_key AND f2.field_name = 'journal' AND f2.field_value LIKE '%IEEE%'
GROUP BY a.name
ORDER BY pub_count DESC ,author_name ASC 
LIMIT 50;
