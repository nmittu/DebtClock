var url = "http://www.brillig.com/debt_clock/";

function load(){
	var XHR = new XMLHttpRequest();
	XHR.open("GET", url, false);
	XHR.send();
	
	var found = XHR.responseText.match(/<IMG.*ALT="(\$(\d| |,|\.)*) ">/);
	
	//console.log(found[1].replace(/ /g,''));
	
	var isGreen = false;
	
	if(localStorage.getItem("lastDebt")){
		if(parseFloat(localStorage.getItem("lastDebt")) > parseFloat(found[1].replace(/ /g,'').replace(/(,|$)/, ''))){
			isGreen = true;
		}
	}
	
	localStorage.setItem("lastDebt", found[1].replace(/ /g,'').replace(/(,|$)/, ''));
	
	var debtString = "";
	var parts = found[1].replace(/ /g,'').split(",");
	
	debtString = parts.slice(0, parts.length/2+1).join(",");
	debtString+=",\n";
	debtString += parts.slice(parts.length/2+1, parts.length).join(",");
	
	//console.log(found[1].replace(/ /g,''));
	//console.log(debtString);
	
	var dictionary = {
		KEY_DEBT: debtString,
		KEY_IS_GREEN: isGreen
	};
	
	Pebble.sendAppMessage(dictionary,
		function(e){
			console.log('Index Sent');
		},
		function(e){
			console.log('Error Sending Weather To Pebble!');
		}
	);
}

Pebble.addEventListener('ready',
  function(e){
    console.log('PebbleKit JS ready!');
		
		load();
		setInterval(load, 5000);
	}
);