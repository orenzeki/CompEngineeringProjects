SELECT p.year, a1.name, COUNT(*) AS count
FROM authored a, author a1, publication p, 
    (SELECT b.year, MAX(b.cnt) AS cnt2
     FROM (SELECT year, a1.name, COUNT(*) AS cnt
       	   FROM authored a, author a1, publication p
	   WHERE a.pub_id = p.pub_id AND a1.author_id = a.author_id AND 
   		 p.year >= 1940 AND p."year" <= 1990
  	   GROUP BY year, a1.name
 	   ORDER BY year ASC, cnt desc, a1.name desc) AS b
     GROUP BY year
     ORDER BY year ASC) AS d
WHERE a.pub_id = p.pub_id AND a1.author_id = a.author_id AND 
      p.year >= 1940 AND p.year <= 1990 AND d.year = p.year
GROUP BY p.year, a1.name, d.cnt2
HAVING COUNT(*) >= d.cnt2
ORDER BY year, a1.name ASC;
