
/*  rate slider */
const container_b = document.querySelector('#slider__box_b');
const btn_b = document.querySelector('#slider__btn_b');
const color_b = document.querySelector('#slider__color_b');
const tooltip_b = document.querySelector('#slider__tooltip_b');

const container_g = document.querySelector('#slider__box_g');
const btn_g = document.querySelector('#slider__btn_g');
const color_g = document.querySelector('#slider__color_g');
const tooltip_g = document.querySelector('#slider__tooltip_g');

const container_r = document.querySelector('#slider__box_r');
const btn_r = document.querySelector('#slider__btn_r');
const color_r = document.querySelector('#slider__color_r');
const tooltip_r = document.querySelector('#slider__tooltip_r');


dragElement_b = (target, btn_b) => {
  target.addEventListener('mousedown', (e) => {
      onMouseMove_b(e);
      window.addEventListener('mousemove', onMouseMove_b);
      window.addEventListener('mouseup', onMouseUp_b);
  });

  onMouseMove_b = (e) => {
      if (rgb_wait == 0) {
        blynk_update_val();
      }
      e.preventDefault();
      let targetRect = target.getBoundingClientRect();
      let x = e.pageX - targetRect.left + 10;
      if (x > targetRect.width) { x = targetRect.width};
      if (x < 0){ x = 0};
      btn_b.x = x - 10;
      btn_b.style.left = btn_b.x + 'px';

      // get the position of the button inside the container (%)
      let percentPosition = (btn_b.x + 10) / targetRect.width * 100;
      
      // color width = position of button (%)
      color_b.style.width = percentPosition + "%";

      // move the tooltip when button moves, and show the tooltip
      tooltip_b.style.left = btn_b.x - 5 + 'px';
      tooltip_b.style.opacity = 1;

      // show the percentage in the tooltip
      tooltip_b.textContent = Math.round(percentPosition) + '%';
  };

  onMouseUp_b  = (e) => {
      window.removeEventListener('mousemove', onMouseMove_b);
      tooltip_b.style.opacity = 0;

      btn_b.addEventListener('mouseover', function() {
        tooltip_b.style.opacity = 1;
      });
      
      btn_b.addEventListener('mouseout', function() {
        tooltip_b.style.opacity = 0;
      });
  };
};

dragElement_b(container_b, btn_b);


dragElement_g = (target, btn_g) => {
  target.addEventListener('mousedown', (e) => {
      onMouseMove_g(e);
      window.addEventListener('mousemove', onMouseMove_g);
      window.addEventListener('mouseup', onMouseUp_g);
  });

  onMouseMove_g = (e) => {
      if (rgb_wait == 0) {
        blynk_update_val();
      }
      e.preventDefault();
      let targetRect = target.getBoundingClientRect();
      let x = e.pageX - targetRect.left + 10;
      if (x > targetRect.width) { x = targetRect.width};
      if (x < 0){ x = 0};
      btn_g.x = x - 10;
      btn_g.style.left = btn_g.x + 'px';

      // get the position of the button inside the container (%)
      let percentPosition = (btn_g.x + 10) / targetRect.width * 100;
      
      // color width = position of button (%)
      color_g.style.width = percentPosition + "%";

      // move the tooltip when button moves, and show the tooltip
      tooltip_g.style.left = btn_g.x - 5 + 'px';
      tooltip_g.style.opacity = 1;

      // show the percentage in the tooltip
      tooltip_g.textContent = Math.round(percentPosition) + '%';
  };

  onMouseUp_g  = (e) => {
      window.removeEventListener('mousemove', onMouseMove_g);
      tooltip_g.style.opacity = 0;

      btn_g.addEventListener('mouseover', function() {
        tooltip_g.style.opacity = 1;
      });
      
      btn_g.addEventListener('mouseout', function() {
        tooltip_g.style.opacity = 0;
      });
  };
};

dragElement_g(container_g, btn_g);




dragElement_r = (target, btn_r) => {
  target.addEventListener('mousedown', (e) => {
      onMouseMove_r(e);
      window.addEventListener('mousemove', onMouseMove_r);
      window.addEventListener('mouseup', onMouseUp_r);
  });

  onMouseMove_r = (e) => {
      if (rgb_wait == 0) {
        blynk_update_val();
      }
      e.preventDefault();
      let targetRect = target.getBoundingClientRect();
      let x = e.pageX - targetRect.left + 10;
      if (x > targetRect.width) { x = targetRect.width};
      if (x < 0){ x = 0};
      btn_r.x = x - 10;
      btn_r.style.left = btn_r.x + 'px';

      // get the position of the button inside the container (%)
      let percentPosition = (btn_r.x + 10) / targetRect.width * 100;
      
      // color width = position of button (%)
      color_r.style.width = percentPosition + "%";

      // move the tooltip when button moves, and show the tooltip
      tooltip_r.style.left = btn_r.x - 5 + 'px';
      tooltip_r.style.opacity = 1;

      // show the percentage in the tooltip
      tooltip_r.textContent = Math.round(percentPosition) + '%';
  };

  onMouseUp_r  = (e) => {
      window.removeEventListener('mousemove', onMouseMove_r);
      tooltip_r.style.opacity = 0;

      btn_r.addEventListener('mouseover', function() {
        tooltip_r.style.opacity = 1;
      });
      
      btn_r.addEventListener('mouseout', function() {
        tooltip_r.style.opacity = 0;
      });
  };
};

dragElement_r(container_r, btn_r);


/*  play button  


playBtn.addEventListener('click', function(e) {
  e.preventDefault();
  pause.classList.toggle('visibility');
  play.classList.toggle('visibility');
  playBtn.classList.toggle('shadow');
  wave1.classList.toggle('paused');
  wave2.classList.toggle('paused');
});
*/