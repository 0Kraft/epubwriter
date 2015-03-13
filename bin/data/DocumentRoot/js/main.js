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
			
		var mydiv2 = document.getElementById("wrapper");
		mydiv2.style.display = 'block';
		
		document.getElementById("uploadFile").value = "";
	
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
	
	$('#addchapter').on('click', function() {
		
		var mydiv = document.getElementById("addchapter-input");
		mydiv.style.display = (mydiv.style.display=='inline'?'none':'inline'); 
		
		document.getElementById("addchaptertitle").focus();
				
    });
	
	$('#addchaptertitlesubmit').on('click', function() {
	
		var $this = $(this);
        JSONRPCClient.call('add-chapter',
            $('#addchaptertitle').val(),
            function(result) {},
            function(error) {
                addError(error);
            });
		
		var mydiv = document.getElementById("addchapter-input");
		mydiv.style.display = (mydiv.style.display=='inline'?'none':'inline'); 
				
    });
	
	$('#a2').on('click', function() {
		
		var mydiv = document.getElementById("contentstatus");
		var mya = document.getElementById("content3"); 
		
		mya.style.display = (mya.style.display=='inline'?'none':'inline'); 
		
		if(mya.style.display=='inline') {
		mydiv.style.display = 'inline';
		document.getElementById("a2").innerHTML = 'c_________reate new epub';
		document.getElementById("content2").style.display = 'none';
		document.getElementById("a1").innerHTML = 'load epub';
		}else{
		document.getElementById("a2").innerHTML = 'create new epub';
		
				
		}
		
		document.getElementById("newepubtitle").focus();
				
    });
	
	$('#a1').on('click', function() {
		
		var mydiv = document.getElementById("contentstatus");
		var mya = document.getElementById("content2"); 
		
				
		mya.style.display = (mya.style.display=='inline'?'none':'inline'); 
		
		if(mya.style.display=='inline') {
		mydiv.style.display = 'inline';
		document.getElementById("a1").innerHTML = 'l__________oad epub';
		document.getElementById("content3").style.display = 'none';
		document.getElementById("a2").innerHTML = 'create new epub';
		}else{
		document.getElementById("a1").innerHTML = 'load epub';
		}
		
		document.getElementById("newepubtitle").focus();
		
				
		
		
		
				
    });
	
	$('#newepubtitlesubmit').on('click', function() {
	
		var $this = $(this);
        JSONRPCClient.call('new-epub',
            $('#newepubtitle').val(),
            function(result) {},
            function(error) {
                addError(error);
            });
		
		var mydiv = document.getElementById("content3");
		mydiv.style.display = (mydiv.style.display=='inline'?'none':'inline'); 
		
		var mydiv2 = document.getElementById("wrapper");
		mydiv2.style.display = 'block';
				
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
	
		document.getElementById("contentstatus").style.display = 'inline';
		document.getElementById("content").style.display = 'inline';
				
		document.getElementById("dropdown-label").style.display = 'inline';
		document.getElementById("wrapper").style.display = 'block';
		document.getElementById("wrapper").style.height = (window.innerHeight * 0.9) + "px";

		document.getElementById("epubtitle").innerHTML = evt.params["value"];
		document.getElementById("epubtitlepre").innerHTML = " &#9679; current epub: ";

	
			
	}
	else if (evt.method == "set-dropdown")
   	{
			
	var element = document.getElementById("test-dropdown");
    element.value = evt.params["value"];
		
	}
	else if (evt.method == "zipready")
   	{
	
	
	sleep(1000);
	
    document.getElementById("downloader").setAttribute('href', document.getElementById("epubtitle").innerHTML + ".epub");
	document.getElementById("downloader").click();

	
		
	}else if(evt.method == "addimageready")
	{
	
		var imgpath = '<div style=\"width:100%;\"><img style=\"width:100%;\" alt=\"img\" src=\"../images/' + evt.params["value"] + '\"/></div>';
		editor.focus();
		editor.composer.commands.exec("insertHTML", imgpath);
		document.getElementById("imguploadFile").value = "";
	
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
