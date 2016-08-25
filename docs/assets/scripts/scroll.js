window.onscroll = function() {
  var myPosition = (window.pageYOffset || document.getElementsByTagName("body")[0].scrollTop || document.body.scrollTop || 0) +
    document.getElementById('header').clientHeight;
  var list = document.getElementsByClassName("panel");
  var divPositions = [];
  for (j = 0; j < list.length; j++) {
    divPositions.push(list.item(j).offsetTop);
  }
  var i = 0;
  while (i + 1 < list.length && myPosition >= divPositions[i + 1]) {
    i += 1;
  }

  var buttons = document.getElementsByClassName("navbutton");
  for (j = 0; j < list.length - 1; j++) {
    buttons[j].style.backgroundColor = "";
  }
  if (i > 0) {
    buttons[i - 1].style.backgroundColor = "#115e33";
  }
}
