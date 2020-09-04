select 
	yearly_flight_count.plane_tail_number, yearly_flight_count.year, (yearly_flight_count.cnt*1.0/daily_flight_count.cnt2) as daily_avg
	from 
	 	(select 
	 		fr.plane_tail_number, fr."year", count(*) as cnt
	  	 from 
			flight_reports fr
	  	 where 
			fr.is_cancelled = 0
	  	 group by 
			fr.plane_tail_number, fr."year") as yearly_flight_count,
	  	(select 
			non_cancelled_flights.plane_tail_number, non_cancelled_flights.year, count(*) as cnt2
	   	 from 
			(select 
				fr.plane_tail_number, fr.year, count(*)
	 	 	 from 
				flight_reports fr
	 	 	 where 
				is_cancelled = 0 
	 	 	 group by 
				fr.plane_tail_number, fr."month", fr."day", fr."year") as non_cancelled_flights
	   	  group by 
			non_cancelled_flights.plane_tail_number, non_cancelled_flights.year) as daily_flight_count
where 
	yearly_flight_count.plane_tail_number = daily_flight_count.plane_tail_number and 
	yearly_flight_count.year = daily_flight_count.year and 
	yearly_flight_count.cnt*1.0/daily_flight_count.cnt2 >5
order by 
yearly_flight_count.plane_tail_number, yearly_flight_count.year asc;
