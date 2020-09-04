select distinct 
	ac.airline_name, ac.airline_code, (avg(fr.departure_delay)) as avg_delay
from 
	airline_codes ac, flight_reports fr
where 
	fr."year" = 2018 and ac.airline_code = fr.airline_code and fr.is_cancelled = 0
group by 
	ac.airline_name, ac.airline_code
order by 
	ac.airline_name, avg_delay asc;
