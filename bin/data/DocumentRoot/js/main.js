// =============================================================================
//
// Copyright (c) 2009-2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================

var JSONRPCClient; ///< The core JSONRPC WebSocket client.

$(document).ready(function() {
    // Initialize our JSONRPCClient
    JSONRPCClient = new $.JsonRpcClient({
        ajaxUrl: getDefaultPostURL(),
        socketUrl: getDefaultWebSocketURL(), // get a websocket for the localhost
        onmessage: onWebSocketMessage,
        onopen: onWebSocketOpen,
        onclose: onWebSocketClose,
        onerror: onWebSocketError
    });
                  
    initializeButtons();
		
    // Dummy call to open the websocket
    JSONRPCClient.call('client',
                        'open-websocket',
                        function(result) {},
                        function(error) {
                            addError(error);
                        });
});

function addError(error) {
    console.log(error);
}

function onWebSocketOpen(ws) {
    console.log("on open");
    console.log(ws);
}

function onWebSocketMessage(evt) {
    //console.log("on message:");
    //console.log(evt.data);

    try {
        var json = JSON.parse(evt.data);

        if (json.module == "Server") {
            handleServerEvent(json);
        } else {
            console.log("Unknown Module: " + json.module);
            console.log(json);
        }
    } catch (e) {
        console.log("Unknown Websocket Data Type");
        console.log(e);
        console.log(evt.data);
    }
}

function onWebSocketClose() {
    console.log("on close");
}

function onWebSocketError() {
    console.log("on error");
}

function initializeButtons() {

	
	$(document).on('keypress','#textarea', function() {
	
	var $this = $(this);
	editor2.setValue($('#textarea').val(),true);
	
	});
	
	$(document).on('keypress','#textarea2', function() {
	
		
	});
	
	$(document).on('click','#textarea2', function() {
	
	var $this = $(this);
	editor2.setValue($('#textarea').val(),true);
	
	});
	
	
	$(window).on('resize', function(){
      var win = $(this); 
	  
		document.getElementById("wrapper").style.height = (window.innerHeight * 0.9) + "px";
	  
	});
	
	 		
	$(document).on('change','#uploadFile', function() {
		var $this = $(this);
        JSONRPCClient.call('set-epubname',
            $('#uploadFile').val(),
            function(result) {},
            function(error) {
                addError(error);
            });
	
	});
	
	$(document).on('change','#test-dropdown', function() {
		var $this = $(this);
        JSONRPCClient.call('test-dropdown',
            $('#test-dropdown').val(),
            function(result) {},
            function(error) {
                addError(error);
            });
	
	});

	
    // Text-Area-Submit
    $('#textarea-button').on('click', function() {
        var $this = $(this);
        JSONRPCClient.call('textarea',
            $('#textarea').val(),
            function(result) {},
            function(error) {
                addError(error);
            });
	 });
			
	$('#zip-epub').on('click', function() {
	
		JSONRPCClient.notify('zip-epub');
			
    });

   	
	$('#aU').on('click', function() {
        var $this = $(this);
        JSONRPCClient.notify('Writer');
		
	
    });

    // Drop-down
    // Use the button to send the selected item name
    $('#test-dropdown-button').on('click', function() {
        JSONRPCClient.call('test-dropdown',
            $('#test-dropdown').val(),
            function(result) {},
            function(error) {
                addError(error);
            });
			
	});

}

// Get the value from the JSON params and assign its value to correct UI element
function handleServerEvent(evt) {
    // Slider
    if (evt.method == "textarea")
    {
		editor.setValue(evt.params["value"],true);
		editor2.setValue(evt.params["value"],true);
		//$('#textarea').val(evt.params["value"]);
	}
	// Dropdown - populate it
    else if (evt.method == "test-dropdown")
    {
		// Get the size of params and go through each to add its value to the dropdown
        for(var i = 0; i < evt.params.length; i++) {
            var option = document.createElement('option');
		
            option.text = option.value = evt.params["item", i];
            document.getElementById("test-dropdown").add(option);
        }
    }
	// Dropdown - remove all elements
	else if (evt.method == "test-dropdown-removeAll")
   	{
		removeOptions(document.getElementById("test-dropdown"));
	}
	else if (evt.method == "reset")
   	{
		//show('content2','a1');
		//show('content' ,'aU');
		
		document.getElementById("dropdown-label").style.display = 'inline';
		document.getElementById("wrapper").style.display = 'block';
		document.getElementById("wrapper").style.height = (window.innerHeight * 0.9) + "px";

		document.getElementById("epubtitle").innerHTML = evt.params["value"];
		document.getElementById("epubtitlepre").innerHTML = " &#9679; current epub: ";

		editor2.setValue(document.getElementById("textarea").value,true); // not sure?
			
	}
	else if (evt.method == "set-dropdown")
   	{
			
	var element = document.getElementById("test-dropdown");
    element.value = evt.params["value"];
		
	}
	else if (evt.method == "zipready")
   	{
	document.getElementById("downloader").setAttribute('href', document.getElementById("epubtitle").innerHTML + ".epub");
	document.getElementById("downloader").click();
	
	}

    console.log(evt);
}

// Remove all elements from dropdown menu
function removeOptions(selectbox)
{
    for(var i = selectbox.options.length - 1; i >= 0; i--) {
        selectbox.remove(i);
    }
}
