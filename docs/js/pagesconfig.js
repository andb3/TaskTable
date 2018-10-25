var slider = document.getElementById("rowRange");
var output = document.getElementById("rowText");
var table_holder = document.getElementById("input_holders");

var tableHTML = '<p class="row_name_text">Row <span class="rowname"></span></p> <div class="textinput_holder"> <input type="text" class="textinput" placeholder="Name"></div>';

output.innerHTML = slider.value; // Display the default slider value

// Update the current slider value (each time you drag the slider handle)
slider.oninput = function() {
    output.innerHTML = this.value;
    createRows(this.value);
}

function rangeColor(input) {
  var wrp = document.createElement('div'),
    preBar = document.createElement('p'),
    min = parseInt(input.min, 10),
    max = parseInt(input.max, 10),
    range = max - min,
    getVal = function() {
      var w = parseInt(input.clientWidth, 10),
        t = ~~(w * (parseInt(input.value, 10) - min) / range);
      return t;
    };
  wrp.className = 'barCnt';
  preBar.className = 'preBar';

  input.className = input.className.length ? (input.className + ' colorized') : 'colorized';
  input.parentNode.replaceChild(wrp, input);

  wrp.appendChild(input);
  wrp.appendChild(preBar);



  input.addEventListener('input', function() {

    preBar.style.width = getVal() + 'px';
  });

  preBar.style.width = getVal() + 'px';
}

function createRows(amount){

  if(table_holder == null){
    console.log("table_holder is null");
  }

  table_holder.innerHTML = "";


  var divArray = new Array();
  for(var i = 0; i<amount; i++){
    divArray[i] = document.createElement('div');
    divArray[i].className = "input";
    divArray[i].innerHTML = tableHTML;
    var rownum = divArray[i].getElementsByClassName("rowname")[0];
    rownum.innerHTML += i+1;
    table_holder.appendChild(divArray[i]);

  }
}


rangeColor(slider);
createRows(3);
//TODO: get previous amount and pass to createRows, further get previous names and other settings, and keep previous inputs on slider shift
