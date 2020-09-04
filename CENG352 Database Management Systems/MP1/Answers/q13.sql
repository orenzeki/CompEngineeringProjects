select 
	monday.airline_name, monday_flights, sunday_flights
from 
	(select 
		ac.airline_name, count(*) as monday_flights
 	 from 
 	 	flight_reports fr, airline_codes ac
	 where 
	 	fr.weekday_id = 1 and fr.airline_code = ac.airline_code and fr.is_cancelled = 0
	 group by 
	 	ac.airline_name) as monday,
    (select 
    	ac.airline_name, count(*) as sunday_flights
	 from 
	 	flight_reports fr, airline_codes ac
	 where 
	 	fr.weekday_id = 7 and fr.airline_code = ac.airline_code and fr.is_cancelled = 0
	 group by 
	 	ac.airline_name) as sunday
where 
	monday.airline_name = sunday.airline_name
order by 
	monday.airline_name asc;
