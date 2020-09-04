select 
	ac.airline_name, cancelled_flights.cnt2*100.0/count(*) as percentage
from 
	flight_reports fr, airline_codes ac, (select 
											ac1.airline_code, count(*) as cnt2
										  from 
										  	flight_reports fr1, airline_codes ac1
										  where 
										  	fr1.airline_code = ac1.airline_code and fr1.origin_city_name like 'Boston%' and 
										    fr1.is_cancelled = 1
										  group by 
										  	ac1.airline_code) as cancelled_flights
where 
	fr.airline_code = ac.airline_code and fr.origin_city_name like 'Boston%' and 
	cancelled_flights.airline_code = fr.airline_code 
group by 
	ac.airline_code, cancelled_flights.cnt2
having 
	cancelled_flights.cnt2 > count(*)*0.1
order by 
	percentage desc;
