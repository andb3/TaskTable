var rangeCache = document.getElementsByClassName("range");

var rangehtml = '<p id="rangetext">Range <span class="rangenumber" id="rangenumber"></span></p><input type="time" name="starttime"  id="starttime" class="starttime"><p id="rangetext">-</p><input type="time" name="endtime" id="endtime" class="endtime"><div class="range_row" id="range_row"><select id="rangedropdown"></select></div><button class="remove_range_button"><i class="material-icons">clear</i></button>';

function resetRanges(daysToSet){
  var rangeholder = document.getElementById("range_holder");
  rangeholder.innerHTML = "";

  setRanges(daysToSet);
}

function getRanges(){
  updateRangeCache();
  setTimes();
  resetRanges();
}

function newRange(){
  var rangeholder = document.getElementById("range_holder");

  var appDiv = document.createElement("DIV");
  appDiv.setAttribute("class", "range")
  appDiv.innerHTML = rangehtml;

  var rangenum = appDiv.getElementsByClassName("rangenumber")[0];
  rangenum.innerHTML = rangeholder.childElementCount + 1;

  rangeholder.appendChild(appDiv);

  var removebutton = appDiv.getElementsByClassName("remove_range_button")[0];

  var elementToRemove = rangeholder.childElementCount - 1;

  removebutton.addEventListener("click", function(e) {
    //e.stopPropagation();
    removeRange(elementToRemove);
  });

  setRangeDropdowns();
}

function removeRange(toRemove){


  var rangeholder = document.getElementById("range_holder");

  console.log("removing element " + toRemove + " of " + rangeholder.childElementCount);

  rangeholder.removeChild(rangeholder.children[toRemove]);
  //rangeholder.removeChild(rangeholder.childNodes[toRemove]);


  var ranges = rangeholder.getElementsByClassName("rangenumber");

  for (var i = toRemove; i<ranges.length; i++){
    ranges[i].innerHTML = i+1;
  }
}

function checkTimes(){
  updateRangeCache();

}

function updateRangeCache(){
  rangeCache = document.getElementsByClassName("range");
}

function setRanges(daysToSet) {
  //get days, show alerts

  if(daysToSet>-1){
  }else if (daysToSet==-3||daysToSet==-2) {
  }else{
  }


  //for(var i = 0; i<)


}
