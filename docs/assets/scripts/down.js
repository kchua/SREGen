document.getElementsByClassName("title")[0].onclick = function() {
  scrollToElement(1000, document.getElementsByClassName("splash")[0]);
}

document.getElementById('down').onclick = function() {
  scrollToElement(1000, document.getElementsByClassName('intro')[0]);
}

document.getElementsByTagName("button").namedItem("introButton").onclick = function() {
  scrollToElement(1000, document.getElementsByClassName('intro')[0]);
}

document.getElementsByTagName("button").namedItem("instructButton").onclick = function() {
  scrollToElement(1000, document.getElementsByClassName('instructions')[0]);
}

document.getElementsByTagName("button").namedItem("samplesButton").onclick = function() {
  scrollToElement(1000, document.getElementsByClassName('samples')[0]);
}

function scrollToElement(duration, element) {
  const scrollHeight = getTop(element) - document.getElementById('header').clientHeight + 1,
        start = window.scrollY,
        cosParameter = scrollHeight / 2;
  var   scrollCount = 0;
        oldTimestamp = performance.now();
  function step(newTimestamp) {
    var newTimestamp = performance.now();
    scrollCount += Math.PI / (duration / (newTimestamp - oldTimestamp));
    if (scrollCount >= Math.PI) {
      window.scrollTo(0, start + scrollHeight);
      return;
    }
    window.scrollTo(0, start + scrollHeight - Math.round(cosParameter * (1 + Math.cos(scrollCount))));
    oldTimestamp = newTimestamp;
    window.requestAnimationFrame(step);
  }
  window.requestAnimationFrame(step);
}

function getTop(element) {
  return element.getBoundingClientRect().top;
}
