select 
	toBoston.year, toBoston.airline_code, (toBoston.cnt) as boston_flight_count, 
	(toBoston.cnt*100.0/toAll.cnt2) as boston_flight_percentage
from 
	(select 
		fr.year, fr.airline_code, (count(*)) as cnt  
	 from 
	 	flight_reports fr
	 where 
	 	fr.dest_city_name like 'Boston%' and fr.is_cancelled = 0
	 group by 
	 	fr.year, fr.airline_code) as toBoston,
	(select 
		fr.year, fr.airline_code, (count(*)) as cnt2 
	 from 
	 	flight_reports fr
	 where 
	 	fr.is_cancelled = 0
	 group by 
	 	fr.year, fr.airline_code) as toAll
where 
	toBoston.year = toAll.year and toBoston.airline_code = toAll.airline_code
group by 
	toBoston.year, toBoston.airline_code, boston_flight_count, boston_flight_percentage
having 
	toBoston.cnt*100.0/toAll.cnt2 > 1
order by 
	toBoston.year, toBoston.airline_code;



