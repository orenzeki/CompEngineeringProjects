SELECT CONCAT(p.year, '-', p.year+10) AS decade, SUM(a.cnt) AS total
FROM (SELECT p.year, COUNT(p.pub_id) AS cnt
      FROM publication p
      WHERE p.year >= 1940
      GROUP BY p.year) AS a, 
     (SELECT p.year, COUNT(p.pub_id) AS cnt
      FROM publication p
      WHERE p.year >= 1940
      GROUP BY p.year) AS p
WHERE a.year >= p.year AND a.year < p.year + 10
GROUP BY p.year
ORDER BY p.year
