var rangeCache = document.getElementsByClassName("range");

var rangehtml = '<p id="rangetext">Range <span class="rangenumber" id="rangenumber"></span></p><input type="time" name="starttime"  id="starttime" class="starttime"><p id="rangetext">-</p><input type="time" name="endtime" id="endtime" class="endtime"><div class="range_row" id="range_row"><select id="rangedropdown"></select></div>';

function resetRanges(){

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

  setRangeDropdowns();
}

function checkTimes(){
  updateRangeCache();

}

function updateRangeCache(){
  rangeCache = document.getElementsByClassName("range");
}
