/* eslint-disable no-unused-lets */
$(document).ready(() => {
  const Site = {
    init () {
      // connect ros to the rosbridge websockets server
      initRosWeb()

      if (getCookie('serialType')) {
        appendToConsole('Found cookie!')
        $('#serial-type').text(getCookie('serialType'))
      }

      // only for ARM and ROVER (for now)
      if (window.location.pathname != '/science') {
        $('#front-camera-stream-btn').on('click', function (event) {
          event.preventDefault()
          // click makes it checked during this time, so trying to enable
          if ($('#front-camera-stream-btn').is(':checked')) {
            requestTask(
              'camera_stream',
              1,
              '#front-camera-stream-btn',
              function (msgs) {
                console.log('front camera ON msgs:', msgs)
                if (msgs[1].includes('Started camera_stream')) {
                  $('img#camera-feed')[0].src =
                    'http://' +
                    getRoverIP() +
                    ':8080/stream?topic=/cv_camera/image_raw'
                  $('#front-camera-stream-btn').addClass('rotateimg180')
                } else {
                  appendToConsole('Failed to open stream')
                  $('#front-camera-stream-btn')[0].checked = false
                }
              },
              '/dev/ttyFrontCam'
            )
          } else {
            requestTask(
              'camera_stream',
              0,
              '#front-camera-stream-btn',
              function (msgs) {
                console.log('front camera OFF msgs:', msgs)
                if (msgs[1].includes('Stopped camera_stream')) {
                  // succeeded to close stream
                  $('img#camera-feed')[0].src =
                    '../static/images/stream-offline.jpg'
                  $('#front-camera-stream-btn').removeClass('rotateimg180')
                } else {
                  // failed to close stream
                  // $('img#camera-feed')[0].src =
                  // 'http://' + getRoverIP() + ':8080/stream?topic=/cv_camera/image_raw'
                  appendToConsole('Failed to close stream')
                }
              },
              '/dev/ttyFrontCam'
            )
          }
        })

        $('#rear-camera-stream-btn').on('click', function (event) {
          event.preventDefault()
          // click makes it checked during this time, so trying to enable
          if ($('#rear-camera-stream-btn').is(':checked')) {
            requestTask(
              'camera_stream',
              1,
              '#rear-camera-stream-btn',
              function (msgs) {
                console.log('rear camera ON msgs:', msgs)
                if (msgs[1].includes('Started camera_stream')) {
                  $('img#camera-feed')[0].src =
                    'http://' +
                    getRoverIP() +
                    ':8080/stream?topic=/cv_camera/image_raw'
                } else {
                  appendToConsole('Failed to open stream')
                  $('#rear-camera-stream-btn')[0].checked = false
                }
              },
              '/dev/ttyRearCam'
            )
          } else {
            requestTask(
              'camera_stream',
              0,
              '#rear-camera-stream-btn',
              function (msgs) {
                if (msgs[1].includes('Stopped camera_stream')) {
                  console.log('rear camera OFF msgs:', msgs)

                  // succeeded to close stream
                  $('img#camera-feed')[0].src =
                    '../static/images/stream-offline.jpg'
                } else {
                  // failed to close stream
                  appendToConsole('Failed to close stream')
                  // $('img#camera-feed')[0].src =
                  // 'http://' + getRoverIP() + ':8080/stream?topic=/cv_camera/image_raw'
                }
              },
              '/dev/ttyRearCam'
            )
          }
        })

        $('#arm-science-camera-stream-btn').on('click', function (event) {
          event.preventDefault()
          // click makes it checked during this time, so trying to enable
          if ($('#arm-science-camera-stream-btn').is(':checked')) {
            requestTask(
              'camera_stream',
              1,
              '#arm-science-camera-stream-btn',
              function (msgs) {
                console.log('arm/science camera ON msgs:', msgs)
                if (msgs[1].includes('Started camera_stream')) {
                  $('img#camera-feed')[0].src =
                    'http://' +
                    getRoverIP() +
                    ':8080/stream?topic=/cv_camera/image_raw'
                } else {
                  appendToConsole('Failed to open stream')
                  $('#arm-science-camera-stream-btn')[0].checked = false
                }
              },
              '/dev/ttyArmScienceCam'
            )
          } else {
            requestTask(
              'camera_stream',
              0,
              '#arm-science-camera-stream-btn',
              function (msgs) {
                if (msgs[1].includes('Stopped camera_stream')) {
                  console.log('arm science camera OFF msgs:', msgs)

                  // succeeded to close stream
                  $('img#camera-feed')[0].src =
                    '../static/images/stream-offline.jpg'
                } else {
                  // failed to close stream
                  appendToConsole('Failed to close stream')
                  // $('img#camera-feed')[0].src =
                  // 'http://' + getRoverIP() + ':8080/stream?topic=/cv_camera/image_raw'
                }
              },
              '/dev/ttyArmScienceCam'
            )
          }
        })
      }

      this.bindEventHandlers()
    },
    bindEventHandlers () {
      this.eventHandlers.forEach(eventHandler => this.bindEvent(eventHandler))
    },
    bindEvent (e) {
      let intervalId
      const binder = () => {
        $.getJSON(e.route, e.handler)
        intervalId = setInterval(() => {
          $.getJSON(e.route, e.handler)
        }, 100)
        return false
      }
      e.$el.bind(e.event, binder)
      e.$el.bind('mouseup', () => {
        clearInterval(intervalId)
      })
    },
    eventHandlers: [
      {
        $el: $('#btn_pitch_up'),
        event: 'mousedown',
        route: '/mousedown_btn_pitch_up',
        handler: data => {}
      },
      {
        $el: $('#btn_pitch_down'),
        event: 'mousedown',
        route: '/mousedown_btn_pitch_down',
        handler: data => {}
      },
      {
        $el: $('#btn_roll_left'),
        event: 'mousedown',
        route: '/mousedown_btn_roll_left',
        handler: data => {}
      },
      {
        $el: $('#btn_roll_right'),
        event: 'mousedown',
        route: '/mousedown_btn_roll_right',
        handler: data => {}
      },
      {
        $el: $('#btn_claw_open'),
        event: 'mousedown',
        route: '/mousedown_btn_claw_open',
        handler: data => {}
      },
      {
        $el: $('#btn_claw_close'),
        event: 'mousedown',
        route: '/mousedown_btn_claw_close',
        handler: data => {}
      },
      {
        $el: $('#btn_arm_up'),
        event: 'mousedown',
        route: '/mousedown_btn_arm_up',
        handler: data => {}
      },
      {
        $el: $('#btn_arm_down'),
        event: 'mousedown',
        route: '/mousedown_btn_arm_down',
        handler: data => {}
      },
      {
        $el: $('#btn_arm_left'),
        event: 'mousedown',
        route: '/mousedown_btn_arm_left',
        handler: data => {}
      },
      {
        $el: $('#btn_arm_right'),
        event: 'mousedown',
        route: '/mousedown_btn_arm_right',
        handler: data => {}
      },
      {
        $el: $('#btn_arm_backward'),
        event: 'mousedown',
        route: '/mousedown_btn_arm_back',
        handler: data => {}
      },
      {
        $el: $('#btn_arm_forward'),
        event: 'mousedown',
        route: '/mousedown_btn_arm_forward',
        handler: data => {}
      },
      {
        $el: $('#btn_motor1_ccw'),
        event: 'mousedown',
        route: '/mousedown_btn_motor1_ccw',
        handler: data => {}
      },
      {
        $el: $('#btn_motor1_cw'),
        event: 'mousedown',
        route: '/mousedown_btn_motor1_cw',
        handler: data => {}
      },
      {
        $el: $('#btn_motor2_ccw'),
        event: 'mousedown',
        route: '/mousedown_btn_motor2_ccw',
        handler: data => {}
      },
      {
        $el: $('#btn_motor2_cw'),
        event: 'mousedown',
        route: '/mousedown_btn_motor2_cw',
        handler: data => {}
      },
      {
        $el: $('#btn_motor3_ccw'),
        event: 'mousedown',
        route: '/mousedown_btn_motor3_ccw',
        handler: data => {}
      },
      {
        $el: $('#btn_motor3_cw'),
        event: 'mousedown',
        route: '/mousedown_btn_motor3_cw',
        handler: data => {}
      },
      {
        $el: $('#btn_motor4_ccw'),
        event: 'mousedown',
        route: '/mousedown_btn_motor4_ccw',
        handler: data => {}
      },
      {
        $el: $('#btn_motor4_cw'),
        event: 'mousedown',
        route: '/mousedown_btn_motor4_cw',
        handler: data => {}
      },
      {
        $el: $('#btn_motor5_ccw'),
        event: 'mousedown',
        route: '/mousedown_btn_motor5_ccw',
        handler: data => {}
      },
      {
        $el: $('#btn_motor5_cw'),
        event: 'mousedown',
        route: '/mousedown_btn_motor5_cw',
        handler: data => {}
      },
      {
        $el: $('#btn_motor6_ccw'),
        event: 'mousedown',
        route: '/mousedown_btn_motor6_ccw',
        handler: data => {}
      },
      {
        $el: $('a#btn_motor6_cw'),
        event: 'mousedown',
        route: '/mousedown_btn_motor6_cw',
        handler: data => {}
      },
      {
        $el: $('button#ping-button'),
        event: 'mouseup',
        route: '/ping_rover',
        handler: data => {
          console.log(data)
          pingRover(data.ping_msg, data.ros_msg)
        }
      }
    ]
  }

  Site.init()

  function isListenerOpen () {
    return (
      (window.location.pathname == '/rover' &&
        $('#toggle-rover-listener-btn')[0].checked == true) ||
      (window.location.pathname == '/' &&
        $('#toggle-arm-listener-btn')[0].checked == true) ||
      (window.location.pathname == '/science' &&
        $('#science-listener-btn')[0].checked == true)
      // pds Listener
    )
  }

  // select mux channel using mux_select service
  $('#mux-0').mouseup(function () {
    // Rover
    if (isListenerOpen()) {
      appendToConsole("Don't change the mux channel while a listener is open!")
    } else {
      requestMuxChannel('#mux-0', function (msgs) {
        console.log('msgs', msgs)
      })
    }
  })

  $('#mux-1').mouseup(function () {
    // Arm
    if (isListenerOpen()) {
      appendToConsole("Don't change the mux channel while a listener is open!")
    } else {
      requestMuxChannel('#mux-1', function (msgs) {
        console.log('msgs', msgs)
      })
    }
  })

  $('#mux-2').mouseup(function () {
    // Science
    if (isListenerOpen()) {
      appendToConsole("Don't change the mux channel while a listener is open!")
    } else {
      requestMuxChannel('#mux-2', function (msgs) {
        console.log('msgs', msgs)
      })
    }
  })

  $('#mux-3').mouseup(function () {
    // PDS
    if (isListenerOpen()) {
      appendToConsole("Don't change the mux channel while a listener is open!")
    } else {
      requestMuxChannel('#mux-3', function (msgs) {
        console.log('msgs', msgs)
      })
    }
  })

  $('#uart').mouseup(function () {
    $('#serial-type').text('uart')
    setCookie('serialType', 'uart', 3)
    appendToConsole('setting cookie to uart')
  })

  $('#usb').mouseup(function () {
    $('#serial-type').text('usb')
    setCookie('serialType', 'usb', 3)
    appendToConsole('setting cookie to usb')
  })

  // send serial command based on mux channel and current page
  // beware that if choosing a different mux channel than the current page,
  // commands will probably mess something up until this is done in a smart manner
  $('#send-serial-btn').mouseup(function () {
    // b
    let cmd = $('#serial-cmd-input').val()
    let buttonText = $('button#mux').text()
    if (buttonText.includes('Select Device Channel')) {
      appendToConsole(
        'Unable to send serial command. Try opening a mux channel.'
      )
    } else {
      // if the appropriate listener is open, send a command to it
      if (
        buttonText.includes('Rover') &&
        $('#toggle-rover-listener-btn')[0].checked == true
      ) {
        // sendRoverCommand(cmd) // rover commands not yet implemented
      } else if (
        buttonText.includes('Arm') &&
        $('#toggle-arm-listener-btn')[0].checked == true
      ) {
        sendArmCommand(cmd)
      } else if (buttonText.includes('Science')) {
        // science buttons unknown
        // sendScienceCommand(cmd) // science commands not yet implemented
      } else if (buttonText.includes('PDS')) {
        // pds buttons unknown
        // sendPdsCommand(cmd) // pds commands not yet implemented
      }
      // no listener is open, send generic request
      else if (!buttonText.includes('Select Device Channel')) {
        requestSerialCommand(cmd, function (msgs) {
          console.log(msgs)
          if (msgs[0]) {
            console.log('nice')
          } else {
            console.log('not nice')
          }
        })
      }
    }
  })

  $('#serial-cmd-input').on('keyup', function (e) {
    if (e.keyCode == 13) {
      // enter key
      // copy code from above
    }
  })
})
