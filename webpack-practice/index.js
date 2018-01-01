var a = "test";
function test(jj) {
  console.log(jj);
}

setTimeout(function () {
  test(a);
}, 1000 * 10);