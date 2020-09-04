SELECT a.name, count(*) AS pub_count
FROM author a, publication p, field f, field f2
WHERE p.pub_key = f.pub_key AND f.field_name = 'author' AND a.name = f.field_value AND 
      p.pub_key = f2.pub_key AND f2.field_name = 'journal' AND 
      f2.field_value LIKE '%IEEE Trans. Wireless Communications%' AND 
      a.name NOT IN (SELECT DISTINCT a1.name 
		     FROM author a1, publication p, field f, field f2
	   	     WHERE p.pub_key = f.pub_key AND f.field_name = 'author' AND 
			   a1.name = f.field_value AND 
	  	           p.pub_key = f2.pub_key AND f2.field_name = 'journal' AND 
	  	           f2.field_value LIKE '%IEEE Wireless Commun. Letters%')
GROUP BY a.name
HAVING COUNT(*)>=10
ORDER BY pub_count desc ,a.name ASC;

