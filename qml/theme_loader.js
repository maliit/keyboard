.import "keys/key_constants.js" as UI
        
var defaultTheme = {
			"fontColor": UI.fontColor,
			"selectionColor": UI.selectionColor,
			"backgroundColor": UI.backgroundColor,
			"dividerColor": UI.dividerColor,
			"annotationFontColor": UI.annotationFontColor,
			"charKeyColor": UI.charKeyColor,
			"charKeyPressedColor": UI.charKeyPressedColor,
			"actionKeyColor": UI.actionKeyColor,
			"actionKeyPressedColor": UI.actionKeyPressedColor,
			"toolkitTheme": UI.toolbarTheme,
			"popupBorderColor": UI.popupBorderColor,
			"keyBorderEnabled": UI.keyBorderEnabled,
			"charKeyBorderColor": UI.charKeyBorderColor,
			"actionKeyBorderColor": UI.actionKeyBorderColor
		}

var load = function (jsonName){
    var xhr = new XMLHttpRequest();

    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4) {
            if (xhr.status == 200) {
                var currentTheme = fullScreenItem.theme
                var newTheme = JSON.parse(xhr.responseText)
                
                for (var key in newTheme) {
                    if (currentTheme.hasOwnProperty(key)) {
                        currentTheme[key] = newTheme[key]
                    }
                }
                fullScreenItem.theme = currentTheme
                console.log('successfully fetched theme');
            }
            else {
                console.log('failed to fetch theme');
            }
        }
    };

    xhr.open('GET', "styles/ubuntu/themes/" + jsonName + ".json", true);
    xhr.send();

}
