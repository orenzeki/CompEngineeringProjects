SELECT a3.name, COUNT(*) AS collab_count
FROM author a3, (SELECT DISTINCT a.author_id, a2.author_id AS a_id
		 FROM authored a, authored a2, publication p
		 WHERE p.pub_id = a.pub_id AND p.pub_id = a2.pub_id AND 
		       a.author_id <> a2.author_id) AS b
WHERE b.author_id = a3.author_id
GROUP BY a3.name
ORDER BY collab_count desc, a3.name ASC
LIMIT 1000;
