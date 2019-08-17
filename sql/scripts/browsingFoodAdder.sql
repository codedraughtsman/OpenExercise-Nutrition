select 
	foodData.ShortFoodName,  
	count( portions.foodName) numOfEntries,
	foodData.Energy Kj, 
	foodData.Fat, 
	foodData.Protein,
	foodData.Carbohydrateavailable
from foodData
left join portions on foodData.ShortFoodName = portions.foodName
group by foodData.ShortFoodName