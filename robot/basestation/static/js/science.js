// turn table graphics
const twopi = 2 * Math.PI
const pi = Math.PI

run = async () => {
  const canvas = document.getElementById('myCanvas')
  const w = canvas.width
  const h = canvas.height
  const context = canvas.getContext('2d')

  const x = 0.8 // at x=1 circle touches canvas edge
  const r = x * 0.5 * Math.min(w, h)

  // BEGIN: GET WHEEL PARAMS FROM SERVER
  const response1 = await fetch('http://localhost:5000/numSections')
  const n = Number(await response1.text())

  const response2 = await fetch('http://localhost:5000/initialSection')
  const initial_section = Number(await response2.text())
  // END: GET WHEEL PARAMS FROM SERVER

  let i = (n - initial_section) % n

  context.fillStyle = '#FFFFFF'
  context.fillRect(0, 0, w, h)

  drawWheel(context, w, h, r, n, ((-i + n) % n) * (twopi / n))

  const nframes = 100
  const d = twopi / n / nframes

  const button_rpos = document.getElementById('button_rpos')
  window.rotatePos = async () => {
    console.log('rotatePos')
    button_rpos.disabled = true
    button_rneg.disabled = true

    const start = (((-i + n) % n) * twopi) / n
    for (let [a, z] = [start, 0]; z < nframes; [a, z] = [a - d, z + 1]) {
      await timer(1)
      drawWheel(context, w, h, r, n, a)
    }

    i = (i + 1) % n
    drawWheel(context, w, h, r, n, (-i * twopi) / n)

    button_rpos.disabled = false
    button_rneg.disabled = false
    lastRotation = Date.now()
  }

  const button_rneg = document.getElementById('button_rneg')
  window.rotateNeg = async () => {
    console.log('rotateNeg')
    button_rpos.disabled = true
    button_rneg.disabled = true

    const start = (-i * twopi) / n
    for (let [a, z] = [start, 0]; z < nframes; [a, z] = [a + d, z + 1]) {
      await timer(1)
      drawWheel(context, w, h, r, n, a)
    }

    i = (i + n - 1) % n
    drawWheel(context, w, h, r, n, (-i * twopi) / n)

    button_rpos.disabled = false
    button_rneg.disabled = false
    lastRotation = Date.now()
  }

  $('#button_rpos').on('click', function (event) {
    if (!isScienceActivated()) {
      return
    }
    sendRequest('Science', 'tccwstep', function (msgs) {
      console.log('msgs', msgs)
    })
  })

  $('#button_rneg').on('click', function (event) {
    if (!isScienceActivated()) {
      return
    }
    sendRequest('Science', 'tcwstep', function (msgs) {
      console.log('msgs', msgs)
    })
  })
}

/**
 * ctx: context
 * w, h: width and height of canvas
 * r: radius of wheel
 * n: number of sections of wheel
 * a: current angle, 0 <= a <= twopi
 */
const drawWheel = (ctx, w, h, r, n, a) => {
  const mx = w / 2
  const my = h / 2

  // setup
  const oldFillStyle = ctx.fillStyle
  const oldLineWidth = ctx.lineWidth
  const oldFont = ctx.font
  const oldTextAlign = ctx.textAlign

  // fill circle
  ctx.fillStyle = '#DBDBDB'
  ctx.beginPath()
  ctx.arc(mx, my, r, 0, twopi)
  ctx.closePath()
  ctx.fill()

  // outline circle
  ctx.fillStyle = 'black'
  ctx.lineWidth = '3.0'
  ctx.beginPath()
  ctx.arc(mx, my, r, 0, twopi)
  ctx.closePath()
  ctx.stroke()

  // draw special background on section n-1
  ctx.fillStyle = '#F5F5F5'
  ctx.lineWidth = '1.0'
  ctx.beginPath()
  ctx.moveTo(mx + Math.cos(a) * r * 0.7, my + Math.sin(a) * r * 0.7)
  ctx.lineTo(mx + Math.cos(a) * r * 0.95, my + Math.sin(a) * r * 0.95)
  ctx.lineTo(
    mx + Math.cos(a + twopi / n) * r * 0.95,
    my + Math.sin(a + twopi / n) * r * 0.95
  )
  ctx.lineTo(
    mx + Math.cos(a + twopi / n) * r * 0.7,
    my + Math.sin(a + twopi / n) * r * 0.7
  )
  ctx.closePath()
  ctx.fill()
  ctx.stroke()

  // draw sections
  ctx.fillStyle = 'black'
  ctx.lineWidth = '2.0'
  ctx.beginPath()
  for (let z = 0; z < n; z++) {
    let a_ = a - z * (twopi / n)
    let x = Math.cos(a_) * r
    let y = Math.sin(a_) * r
    ctx.moveTo(mx, my)
    ctx.lineTo(mx + x, my + y)
  }
  ctx.stroke()

  // draw numbers
  let oldA = a
  a -= twopi / (n * 2) // add half a section to a
  ctx.font = '18px serif'
  ctx.fillStyle = 'black'
  ctx.textAlign = 'center'
  let indices = [0, 3, 2, 1]
  for (let z = 0; z < n; z++) {
    let number = indices[z]
    let a_ = a - z * (twopi / n)
    let x = Math.cos(a_) * r * 0.8
    let y = Math.sin(a_) * r * 0.8
    ctx.fillText(`${number}`, mx + x, my + y)
  }
  a = oldA

  // draw wheel pointer
  // circle
  ctx.fillStyle = '#525252'
  ctx.lineWidth = '3.0'
  ctx.beginPath()
  ctx.arc(mx, my, r * 0.3, 0, twopi)
  ctx.closePath()
  ctx.fill()
  ctx.stroke()
  // pointer
  ctx.beginPath()
  ctx.moveTo(
    mx + Math.cos(-twopi / n) * r * 0.3,
    my + Math.sin(-twopi / n) * r * 0.3
  )
  ctx.lineTo(mx + Math.cos(-pi / n) * r * 0.5, my + Math.sin(-pi / n) * r * 0.5)
  ctx.lineTo(mx + Math.cos(0) * r * 0.3, my + Math.sin(0) * r * 0.3)
  ctx.closePath()
  ctx.fill()
  ctx.beginPath()
  ctx.moveTo(
    mx + Math.cos(-twopi / n) * r * 0.3,
    my + Math.sin(-twopi / n) * r * 0.3
  )
  ctx.lineTo(mx + Math.cos(-pi / n) * r * 0.5, my + Math.sin(-pi / n) * r * 0.5)
  ctx.lineTo(mx + Math.cos(0) * r * 0.3, my + Math.sin(0) * r * 0.3)
  ctx.stroke()
  // text
  ctx.font = '30px arial'
  ctx.fillStyle = 'white'
  ctx.fillText('current', mx, my - 8)
  ctx.fillText('vial', mx, my + 22)

  // teardown
  ctx.fillStyle = oldFillStyle
  ctx.lineWidth = oldLineWidth
  ctx.font = oldFont
  ctx.textAlign = oldTextAlign
}

const timer = delay => {
  return new Promise(function (resolve) {
    setTimeout(resolve, delay)
  })
}

function isScienceActivated () {
  if (!$('#science-listener-btn').is(':checked')) {
    appendToConsole('Science listener not yet activated!')
    return false
  } else if (!$('#activate-science-btn').is(':checked')) {
    appendToConsole('Science MCU is not yet activated!')
    return false
  }
  return true
}

// convenience UI functions
function toggleOffAllPumps () {
  for (let i = 0; i < 5; i++) {
    $('#pump' + (i + 1) + '-drive-toggle')[0].checked = false
  }
}

function toggleOffAllVibrators () {
  for (let i = 0; i < 6; i++) {
    $('#vibrator' + (i + 1) + '-toggle')[0].checked = false
  }
}

$(document).ready(function () {
  // MCU ping
  $('#ping-science-mcu').on('click', function (event) {
    event.preventDefault()
    sendRequest('Science', 'ping', printErrToConsole)
  })

  $('#ping-odroid').on('click', function (event) {
    appendToConsole('pinging odroid')
    $.ajax('/ping_rover', {
      success: function (data) {
        appendToConsole(data.ping_msg)
        if (!data.ros_msg.includes('Response')) {
          appendToConsole('No response from ROS ping_acknowledgment service')
        } else {
          appendToConsole(data.ros_msg)
        }
      },
      error: function () {
        console.log('An error occured')
      }
    })
    lastCmdSent = new Date().getTime()
  })

  // ROS related stuff
  $('#science-listener-btn').on('click', function (event) {
    event.preventDefault()
    let serialType = $('#serial-type')
      .text()
      .trim()
    // click makes it checked during this time, so trying to enable
    if ($('#science-listener-btn').is(':checked')) {
      if (
        $('#serial-type')
          .text()
          .includes('Serial')
      ) {
        appendToConsole('Select a serial type!')
      }
      // validate UART mode options are correct, let pass if USB mode selected
      else if (
        ($('button#mux')
          .text()
          .includes('Science') &&
          serialType == 'uart') ||
        serialType == 'usb'
      ) {
        requestTask(
          'science_listener',
          1,
          '#science-listener-btn',
          function (msgs) {
            console.log(msgs)
            if (msgs.length == 2) {
              if (msgs[1].includes('already running') || msgs[0] == true) {
                $('#science-listener-btn')[0].checked = true
              } else {
                $('#science-listener-btn')[0].checked = false
              }
            }
          },
          serialType
        )
      } else {
        appendToConsole(
          'UART MODE: Cannot turn science listener on if not in science mux channel!'
        )
      }
    } else {
      // closing arm listener
      requestTask('science_listener', 0, '#science-listener-btn', function (
        msgs
      ) {
        console.log('msgs[0]', msgs[0])
        if (msgs.length == 2) {
          console.log('msgs[1]', msgs[1])
          if (msgs[1].includes('already running')) {
            $('#science-listener-btn')[0].checked = true
          } else {
            $('#science-listener-btn')[0].checked = false
          }
        } else {
          if (msgs[0]) {
            $('#science-listener-btn')[0].checked = true
          } else {
            $('#science-listener-btn')[0].checked = false
          }
        }
      })
    }
  })

  $('#activate-science-btn').on('click', function (event) {
    event.preventDefault()
    // click makes it checked during this time, so trying to enable
    if (!$('#science-listener-btn').is(':checked')) {
      appendToConsole('Science listener not yet activated!')
    } else if ($('#activate-science-btn').is(':checked')) {
      sendRequest('Science', 'activate', function (msgs) {
        console.log('msgs', msgs)
      })
    } else {
      // 'deactivated' needs to be handled differently since it takes 45 secconds
      sendRequest('Science', 'stop', function (msgs) {
        console.log('msgs', msgs)
      })
    }
  })

  $('#ccw-btn').on('click', function (event) {
    if (!isScienceActivated()) {
      return
    }
    sendRequest('Science', 'dccw', function (msgs) {
      console.log('msgs', msgs)
    })
  })

  $('#cw-btn').on('click', function (event) {
    if (!isScienceActivated()) {
      return
    }
    sendRequest('Science', 'dcw', function (msgs) {
      console.log('msgs', msgs)
    })
  })

  // @TODO: these buttons are flipped because of wiring issues
  // fix the wiring, change back the correct values
  $('#elevator-up-btn').on('click', function (event) {
    if (!isScienceActivated()) {
      return
    }
    sendRequest('Science', 'edown', function (msgs) {
      console.log('msgs', msgs)
    })
  })

  $('#elevator-down-btn').on('click', function (event) {
    if (!isScienceActivated()) {
      return
    }
    sendRequest('Science', 'eup', function (msgs) {
      console.log('msgs', msgs)
    })
  })

  let pumpDriveToggles = [
    '#pump1-drive-toggle',
    '#pump2-drive-toggle',
    '#pump3-drive-toggle',
    '#pump4-drive-toggle',
    '#pump5-drive-toggle'
  ]

  for (let i = 0; i < pumpDriveToggles.length; i++) {
    $(pumpDriveToggles[i]).on('click', function (event) {
      event.preventDefault()
      let cmd = 'p' + (i + 1)

      if (!isScienceActivated()) {
        return
      }

      // click makes it checked during this time, so trying to enable
      if ($(pumpDriveToggles[i]).is(':checked')) {
        sendRequest('Science', cmd, function (msgs) {
          console.log('msgs', msgs)
          if (msgs[1].includes(cmd + ' done')) {
            $(pumpDriveToggles[i])[0].checked = true
          } else {
            $(pumpDriveToggles[i])[0].checked = false
          }
        })
      } else {
        // stop all pumps
        sendRequest('Science', 'ps', function (msgs) {
          if (msgs[1].includes('ps done')) {
            toggleOffAllPumps()
          } else {
            appendToConsole('Something went wrong')
          }
        })
      }
    })
  }

  $('#pump-dir-toggle').click(function (event) {
    event.preventDefault()

    if (!isScienceActivated()) {
      return
    }
    // click makes it checked during this time, so trying to enable
    if ($('#pump-dir-toggle').is(':checked')) {
      sendRequest('Science', 'pd1', function (msgs) {
        if (msgs[1].includes('OUT')) {
          appendToConsole('Success')
        } else {
          appendToConsole('Something went wrong')
        }
      })
    } else {
      sendRequest('Science', 'pd0', function (msgs) {
        if (msgs[1].includes('IN')) {
          appendToConsole('Success')
        } else {
          appendToConsole('Something went wrong')
        }
      })
    }
  })

  for (let i = 0; i < 2; i++) {
    $('#led' + (i + 1) + '-toggle').click(function (event) {
      event.preventDefault()

      if (!isScienceActivated()) {
        return
      }

      let cmd = 'led' + (i + 1)

      if ($('#led' + (i + 1) + '-toggle').is(':checked')) {
        sendRequest('Science', cmd, function (msgs) {
          console.log('msgs', msgs)

          if (msgs[1].includes(cmd + ' done')) {
            appendToConsole('Success')
          } else {
            appendToConsole('Something went wrong')
          }
        })
      } else {
        cmd += 's'
        sendRequest('Science', cmd, function (msgs) {
          console.log('msgs', msgs)

          if (msgs[1].includes(cmd + ' done')) {
            appendToConsole('Success')
          } else {
            appendToConsole('Something went wrong')
          }
        })
      }
    })
  }

  for (let i = 0; i < 6; i++) {
    $('#vibrator' + (i + 1) + '-toggle').click(function (event) {
      event.preventDefault()

      if (!isScienceActivated()) {
        return
      }

      let cmd = 'v' + (i + 1)

      if ($('#vibrator' + (i + 1) + '-toggle').is(':checked')) {
        sendRequest('Science', cmd, function (msgs) {
          console.log('msgs', msgs)

          if (msgs[1].includes(cmd + ' done')) {
            appendToConsole('Success')
          } else {
            appendToConsole('Something went wrong')
          }
        })
      } else {
        cmd = 'vs'
        sendRequest('Science', cmd, function (msgs) {
          console.log('msgs', msgs)

          if (msgs[1].includes(cmd + ' done')) {
            appendToConsole('Success')
          } else {
            appendToConsole('Something went wrong')
          }
        })
      }
    })
  }

  $('#drill-max-speed-go-btn').click(function (msgs) {
    if (!isScienceActivated()) {
      return
    }

    sendRequest('Science', 'dgo', function (msgs) {
      console.log('msgs', msgs)

      if (msgs[1].includes('dgo done')) {
        appendToConsole('Success')
        lightUp('#drill-max-speed-go-btn')
        greyOut('#drill-stop-btn')
      } else {
        appendToConsole('Something went wrong')
      }
    })
  })

  $('#set-speed-go-btn').click(function (msgs) {
    if (!isScienceActivated()) {
      return
    }

    let drillSpeed = $('#drill-speed').val()
    let cmd = 'drillspeed ' + drillSpeed
    let requestedSpeed = Number(drillSpeed)

    // invalid range check
    // values under 6% don't actually rotate the drill
    if (requestedSpeed < 6 || requestedSpeed > 100) {
      color('#drill-speed', 'orange')
      appendToConsole('Valid ranges for drill speed: [7, 100]')
      return
    }

    color('#drill-speed', 'white')

    sendRequest('Science', cmd, function (msgs) {
      console.log('msgs', msgs)

      if (msgs[1].includes('drillspeed done')) {
        appendToConsole('Success')
        lightUp('#set-speed-go-btn')
        greyOut('#drill-stop-btn')
      } else {
        appendToConsole('Something went wrong')
      }
    })
  })

  $('#set-time-go-btn').click(function (msgs) {
    if (!isScienceActivated()) {
      return
    }

    let drillTime = $('#drill-time').val()
    let cmd = 'drilltime ' + drillTime
    let requestedTime = Number(drillTime)

    // ensure time at least one second
    if (requestedTime < 1) {
      color('#drill-time', 'orange')
      appendToConsole('Valid ranges for drill speed: [1, ∞)')
      return
    }

    color('#drill-time', 'white')

    // check if drillspeed set
    let drillSpeed = $('#drill-speed').val()

    if (isNumeric(drillSpeed)) {
      cmd += ' ' + drillSpeed
    }

    sendRequest('Science', cmd, function (msgs) {
      console.log('msgs', msgs)

      if (msgs[1].includes('drilltime done')) {
        appendToConsole('Success')
        lightUp('#set-time-go-btn')
        greyOut('#drill-stop-btn')
      } else {
        appendToConsole('Something went wrong')
      }
    })
  })

  $('#drill-stop-btn').click(function (msgs) {
    if (!isScienceActivated()) {
      return
    }

    // drill stop
    sendRequest('Science', 'ds', function (msgs) {
      console.log('msgs', msgs)

      if (msgs[1].includes('ds done')) {
        appendToConsole('Success')
        lightUp('#drill-stop-btn')
        greyOut('#drill-max-speed-go-btn')
        greyOut('#set-speed-go-btn')
        greyOut('#set-time-go-btn')
      } else {
        appendToConsole('Something went wrong')
      }
    })
  })

  $('#elevator-max-speed-go-btn').click(function (msgs) {
    if (!isScienceActivated()) {
      return
    }

    sendRequest('Science', 'ego', function (msgs) {
      console.log('msgs', msgs)

      if (msgs[1].includes('ego done')) {
        appendToConsole('Success')
        lightUp('#elevator-max-speed-go-btn')
        greyOut('#elevator-stop-btn')
      } else {
        appendToConsole('Something went wrong')
      }
    })
  })

  $('#set-feed-go-btn').click(function (msgs) {
    if (!isScienceActivated()) {
      return
    }

    let elevatorFeed = $('#elevator-feed').val()
    let cmd = 'ef ' + elevatorFeed
    let requestedFeed = Number(elevatorFeed)

    // invalid range check
    // values under 10% don't actually move the elevator
    if (requestedFeed < 10 || requestedFeed > 100) {
      color('#elevator-feed', 'orange')
      appendToConsole('Valid ranges for elevator feed: [10, 100]')
      return
    }

    color('#elevator-feed', 'white')

    sendRequest('Science', cmd, function (msgs) {
      console.log('msgs', msgs)

      if (msgs[1].includes('elevatorfeed done')) {
        appendToConsole('Success')
        lightUp('#set-feed-go-btn')
        greyOut('#elevator-stop-btn')
      } else {
        appendToConsole('Something went wrong')
      }
    })
  })

  // elevator - set distance command
  $('#set-distance-go-btn').click(function (msgs) {
    if (!isScienceActivated()) {
      return
    }

    let elevatorDistance = $('#elevator-distance').val()
    let cmd = 'elevatordistance ' + elevatorDistance
    let requestedDistance = Number(elevatorDistance)

    // invalid range check
    // if the system on the MCU were closed loop we would do it based off current position
    // otherwise we assume there will be limit switches for protection
    if (requestedDistance <= 0 || requestedDistance > 15) {
      color('#elevator-distance', 'orange')
      appendToConsole('Valid ranges for elevator feed: (0, 15]')
      return
    }

    color('#elevator-distance', 'white')

    // check if feed set
    let feed = $('#elevator-feed').val()
    feed = Number(feed)

    // anything under 20% is too slow and takes _very_ long time to reach 1 inch distance
    if (isNumeric(feed) && (feed >= 20 && feed <= 100)) {
      cmd += ' ' + feed
    } else {
      color('#elevator-feed', 'orange')
      appendToConsole('Illegal parameter set for feed')
      appendToConsole('Legal range of integers: [10, 100]')
      return
    }

    // if we made it this far then reset the input field to normal background
    color('#elevator-feed', 'white')

    sendRequest('Science', cmd, function (msgs) {
      console.log('msgs', msgs)

      if (msgs[1].includes('elevatordistance done')) {
        appendToConsole('Success')
        lightUp('#set-distance-go-btn')
        greyOut('#elevator-stop-btn')
      } else {
        appendToConsole('Something went wrong')
      }
    })
  })

  $('#elevator-stop-btn').click(function (msgs) {
    if (!isScienceActivated()) {
      return
    }

    sendRequest('Science', 'es', function (msgs) {
      console.log('msgs', msgs)

      if (msgs[1].includes('es done')) {
        appendToConsole('Success')
        lightUp('#elevator-stop-btn')
        greyOut('#elevator-max-speed-go-btn')
        greyOut('#set-feed-go-btn')
        greyOut('#set-distance-go-btn')
      } else {
        appendToConsole('Something went wrong')
      }
    })
  })
})
