var x, rangePickers;
/*look for any elements with the class "time_picker_dropdown":*/
x = document.getElementsByClassName("time_picker_dropdown");

rangePickers = document.getElementsByClassName("range");

for (var i = 0; i < x.length; i++) {
  var j, selElmnt, a, b, c;
  selElmnt = x[i].getElementsByTagName("select")[0];
  /*for each element, create a new DIV that will act as the selected item:*/
  a = document.createElement("DIV");
  a.setAttribute("class", "select-selected");
  a.innerHTML = selElmnt.options[selElmnt.selectedIndex].innerHTML;
  x[i].appendChild(a);
  /*for each element, create a new DIV that will contain the option list:*/
  b = document.createElement("DIV");
  b.setAttribute("class", "select-items select-hide");
  for (var j = 0; j < selElmnt.length; j++) {
    /*for each option in the original select element,
    create a new DIV that will act as an option item:*/
    c = document.createElement("DIV");
    c.innerHTML = selElmnt.options[j].innerHTML;
    c.addEventListener("click", function(e) {
      /*when an item is clicked, update the original select box,
      and the selected item:*/
      var y, i, k, s, h;
      s = this.parentNode.parentNode.getElementsByTagName("select")[0];
      h = this.parentNode.previousSibling;
      for (i = 0; i < s.length; i++) {
        if (s.options[i].innerHTML == this.innerHTML) {
          console.log("selected index: " + s.selectedIndex);
          var oldSelect = s.selectedIndex;
          console.log("selected index: " + oldSelect);

          setTimes(); //before index set

          s.selectedIndex = i;
          h.innerHTML = this.innerHTML;
          y = this.parentNode.getElementsByClassName("same-as-selected");
          for (k = 0; k < y.length; k++) {
            y[k].removeAttribute("class");
          }
          this.setAttribute("class", "same-as-selected");
          getRanges();
          setRanges(i-3, oldSelect);
          //getTimesByDay();
          break;
        }
      }
      h.click();
    });
    b.appendChild(c);
  }
  x[i].appendChild(b);
  a.addEventListener("click", function(e) {
    /*when the select box is clicked, close any other select boxes,
    and open/close the current select box:*/
    e.stopPropagation();
    closeAllSelect(this);
    this.nextSibling.classList.toggle("select-hide");
    this.classList.toggle("select-arrow-active");
  });
}
function closeAllSelect(elmnt) {
  /*a function that will close all select boxes in the document,
  except the current select box:*/
  var x, y, i, arrNo = [];
  x = document.getElementsByClassName("select-items");
  y = document.getElementsByClassName("select-selected");
  for (i = 0; i < y.length; i++) {
    if (elmnt == y[i]) {
      arrNo.push(i)
    } else {
      y[i].classList.remove("select-arrow-active");
    }
  }
  for (i = 0; i < x.length; i++) {
    if (arrNo.indexOf(i)) {
      x[i].classList.add("select-hide");
    }
  }
}
/*if the user clicks anywhere outside the select box,
then close all select boxes:*/
document.addEventListener("click", closeAllSelect);


/********************************************************************/

function setRangeDropdowns(){

  rangePickers = document.getElementsByClassName("range_row");


  var removeElmnt, removeList, selElmnt, a, b, c;
  for (var i = 0; i < rangePickers.length; i++) {

    removeElmnt = rangePickers[i].getElementsByClassName("select-selected");
    removeList = rangePickers[i].getElementsByClassName("select-items select-hide");

    if(removeElmnt.length>0){
      console.log("removing");
      rangePickers[i].removeChild(removeElmnt[0]);
      rangePickers[i].removeChild(removeList[0]);

    }

    //a.innerHTML = ""; /*clear previous html*/
    selElmnt = rangePickers[i].getElementsByTagName("select")[0];
    //selElmnt.innerHTML = ""; /*clear previous html*/
    /*for each element, create a new DIV that will act as the selected item:*/
    a = document.createElement("DIV");
    a.setAttribute("class", "select-selected");
    if(preclasses[selElmnt.selectedIndex] == null || preclasses[selElmnt.selectedIndex] == ""){
      a.innerHTML = /*preclasses[selElmnt.selectedIndex]*/ "No Name";
    }else {
      a.innerHTML = preclasses[selElmnt.selectedIndex];
    }
    //a.innerHTML = /*preclasses[selElmnt.selectedIndex]*/ "Class 1";
    rangePickers[i].appendChild(a);
    /*for each element, create a new DIV that will contain the option list:*/
    b = document.createElement("DIV");
    b.setAttribute("class", "select-items select-hide");
    for (var j = 0; j < preclasses.length; j++) {
      /*for each option in the original select element,
      create a new DIV that will act as an option item:*/
      c = document.createElement("DIV");
      //c.innerHTML = selElmnt.options[j].innerHTML;
      //console.log(preclasses[j]);
      if(preclasses[j] == null || preclasses[j] == ""){
        //console.log("Preclasses not filled");
        c.innerHTML = "No Name";
      }else {
        //console.log("Preclasses filled: " + preclasses[j]);
        c.innerHTML = preclasses[j];
      }
      //c.innerHTML = preclasses[j];
      c.addEventListener("click", function(e) {
        /*when an item is clicked, update the original select box,
        and the selected item:*/
        console.log(this.innerHTML);
        var y, i, k, s, h;
        s = this.parentNode.parentNode.getElementsByTagName("select")[0];
        h = this.parentNode.previousSibling;
        for (i = 0; i < preclasses.length; i++) {
          if (preclasses[i] == this.innerHTML) {
            s.selectedIndex = i;
            h.innerHTML = this.innerHTML;
            y = this.parentNode.getElementsByClassName("same-as-selected");
            for (k = 0; k < y.length; k++) {
              y[k].removeAttribute("class");
            }
            this.setAttribute("class", "same-as-selected");
            break;
          }
        }
        h.click();
      });
      b.appendChild(c);
    }
    rangePickers[i].appendChild(b);
    a.addEventListener("click", function(e) {
      /*when the select box is clicked, close any other select boxes,
      and open/close the current select box:*/
      e.stopPropagation();
      closeAllSelect(this);
      this.nextSibling.classList.toggle("select-hide");
      this.classList.toggle("select-arrow-active");
      setTimes();
    });
  }
}

setPreclasses();
