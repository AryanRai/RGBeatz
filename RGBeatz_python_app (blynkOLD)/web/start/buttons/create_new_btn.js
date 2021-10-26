gsap.registerPlugin(Physics2DPlugin);

document.querySelectorAll('.new_device_button').forEach(button => {

    let getVar = variable => getComputedStyle(button).getPropertyValue(variable);
    const bounding = button.getBoundingClientRect()



    function create_new_device() {

        blynkbutton.style.visibility = "hidden";
        description_para.innerHTML = "Why u still here hUh? yeah YoU.. Im talking to you. Go to the other window and Register your device😉 !Dont close this window!";
        create_new_device_window = window.open('http://' + hostname + ':' + window.location.port + '/create_new_device/create_new_device.html', "Popup", "Spotify");
        create_new_device_window.resizeTo(10000, 10000);
        //create_new_device_window.moveTo(400, 50);
        create_new_device_window.focus(); 
    
        
  }

  



    button.addEventListener('mousemove', e => {

        let dy = (e.clientY - bounding.top - bounding.height / 2) / -1
        let dx = (e.clientX - bounding.left - bounding.width / 2)  / 10

        dy = dy > 10 ? 10 : (dy < -10 ? -10 : dy);
        dx = dx > 4 ? 4 : (dx < -4 ? -4 : dx);

        button.style.setProperty('--rx', dy);
        button.style.setProperty('--ry', dx);

    });

    button.addEventListener('mouseleave', e => {

        button.style.setProperty('--rx', 0)
        button.style.setProperty('--ry', 0)

    });

    button.addEventListener('click', e => {

        if(!button.classList.contains('active')) {

            
            button.classList.add('active');

            gsap.to(button, {
                keyframes: [{
                    '--left-wing-first-x': 50,
                    '--left-wing-first-y': 100,
                    '--right-wing-second-x': 50,
                    '--right-wing-second-y': 100,
                    duration: .2,
                    onComplete() {
                        gsap.set(button, {
                            '--left-wing-first-y': 0,
                            '--left-wing-second-x': 40,
                            '--left-wing-second-y': 100,
                            '--left-wing-third-x': 0,
                            '--left-wing-third-y': 100,
                            '--left-body-third-x': 40,
                            '--right-wing-first-x': 50,
                            '--right-wing-first-y': 0,
                            '--right-wing-second-x': 60,
                            '--right-wing-second-y': 100,
                            '--right-wing-third-x': 100,
                            '--right-wing-third-y': 100,
                            '--right-body-third-x': 60
                        })
                    }
                }, {
                    '--left-wing-third-x': 20,
                    '--left-wing-third-y': 90,
                    '--left-wing-second-y': 90,
                    '--left-body-third-y': 90,
                    '--right-wing-third-x': 80,
                    '--right-wing-third-y': 90,
                    '--right-body-third-y': 90,
                    '--right-wing-second-y': 90,
                    duration: .2
                }, {
                    '--rotate': 50,
                    '--left-wing-third-y': 95,
                    '--left-wing-third-x': 27,
                    '--right-body-third-x': 45,
                    '--right-wing-second-x': 45,
                    '--right-wing-third-x': 60,
                    '--right-wing-third-y': 83,
                    duration: .25
                }, {
                    '--rotate': 55,
                    '--plane-x': -8,
                    '--plane-y': 24,
                    duration: .2
                }, {
                    '--rotate': 40,
                    '--plane-x': 45,
                    '--plane-y': -180,
                    '--plane-opacity': 0,
                    duration: .3,
                    onComplete() {
                        setTimeout(() => {
                            //button.removeAttribute('style');
                            gsap.fromTo(button, {
                                //opacity: 0,
                                //y: -8
                            }, {
                                //opacity: 1,
                                //y: 0,
                                clearProps: true,
                                duration: .3,
                                onComplete() {
                                    //button.classList.remove('active');
                                    ////////////////////// on succcccesssssssssssss
                                    console.log("done khatam finish");
                                    create_new_device();
                                }
                            })
                        }, 2000)
                    }
                }]
            })

            gsap.to(button, {
                keyframes: [{
                    '--text-opacity': 0,
                    '--border-radius': 0,
                    '--left-wing-background': getVar('--primary-darkest'),
                    '--right-wing-background': getVar('--primary-darkest'),
                    duration: .1
                }, {
                    '--left-wing-background': getVar('--primary'),
                    '--right-wing-background': getVar('--primary'),
                    duration: .1
                }, {
                    '--left-body-background': getVar('--primary-dark'),
                    '--right-body-background': getVar('--primary-darkest'),
                    duration: .4
                }, {
                    '--success-opacity': 1,
                    '--success-scale': 1,
                    duration: .25,
                    delay: .25
                }]
            })

        }

    })

});