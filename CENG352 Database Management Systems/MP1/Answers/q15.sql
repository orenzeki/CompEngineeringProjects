select 
	fnl.airport_desc
from 
	(select 
		ac.airport_desc, cnt + cnt2 as trfc
      from 
      	(select 
      		fr.dest_airport_code, count(*) as cnt
         from 
         	flight_reports fr
         where 
         	fr.is_cancelled = 0
	 	 group by 
	 	 	fr.dest_airport_code) as incoming, 
	 (select 
	 	fr.origin_airport_code, count(*) as cnt2
	  from 
	  	flight_reports fr
	  where 
	  	fr.is_cancelled = 0
	  group by 
	  	fr.origin_airport_code) as outgoing, airport_codes ac
	  where 
	  	incoming.dest_airport_code = outgoing.origin_airport_code and ac.airport_code = incoming.dest_airport_code
	  order by 
	  	trfc desc) as fnl
limit 5;



