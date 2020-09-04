select 
	a.year, w.weekday_name, a.cancellation_reason as reason, cnt as number_of_cancellations
from 
	(select 
		fr."year", fr.weekday_id, fr.cancellation_reason, count(*) as cnt
	 from 
	 	flight_reports fr
	 where 
	 	fr.is_cancelled = 1
	 group by 
	 	fr."year", fr.weekday_id, fr.cancellation_reason) as a, 	
	 (select 
	 	fr."year", fr.weekday_id, fr.cancellation_reason, count(*) as cnt2
	 from 
	 	flight_reports fr
	 where 
	 	fr.is_cancelled = 1
	 group by 
	 	fr."year", fr.weekday_id, fr.cancellation_reason) as b,
	 	weekdays w
where 
	a.year = b.year and a.weekday_id = b.weekday_id and  w.weekday_id = b.weekday_id
group by 
	a.year, w.weekday_name, w.weekday_id, a.cancellation_reason, a.cnt
having 
	cnt = max(cnt2)
order by 
	a.year, w.weekday_id asc;
