select  portions.foodName, 
	sum(portions.grams) totalGrams, 
/*	(sum(portions.grams) +0.0) / foodData.Measure factor, 
	foodData.Measure , */

	foodData.Protein *(sum(portions.grams) +0.0) / foodData.Measure proteinTotal, 
	foodData.Fat *(sum(portions.grams) +0.0) / foodData.Measure fatTotal, 
	foodData.Carbohydrateavailable *(sum(portions.grams) +0.0) / foodData.Measure Carbohydratesavailable
from portions  
left join foodData on portions.foodName =foodData.ShortFoodName 
group by portions.foodName