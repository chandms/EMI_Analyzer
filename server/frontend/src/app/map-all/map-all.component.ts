import { Component, OnInit } from '@angular/core';


import 'ol/ol.css';
import * as olsource from 'ol/source';
import Map from 'ol/Map';
import {Tile as TileLayer} from 'ol/layer';
import Overlay from 'ol/Overlay';
import View from 'ol/View';
import { fromLonLat, toLonLat } from 'ol/proj';
import Feature from 'ol/Feature';
import { Point } from 'ol/geom';
import { Vector } from 'ol/layer';
import OSM from 'ol/source/OSM';
import Style from 'ol/style/Style';
import Icon from 'ol/style/Icon';
import VectorSource from 'ol/source/Vector';

import { DeviceService } from '../service/device.service';
import { Device } from '../device/device.component';
import { ActivatedRoute } from '@angular/router';
import {toStringHDMS} from 'ol/coordinate';


@Component({
  selector: 'map-all',
  templateUrl: './map-all.component.html',
  styleUrls: ['./map-all.component.css']
})
export class MapAllComponent implements OnInit {

  constructor(private service: DeviceService,  private route : ActivatedRoute) { }

  map!: Map;
  zoom: number = 12;
  latitude: number = 40.4259;
  longitude: number = -86.9081;
  user_first_name: string ="";
  user_last_name: string ="";
  user_email = "";
  $: any;

  

  ngOnInit(): void {
    this.user_first_name = history.state.firstname;
    this.user_last_name = history.state.lastname;
    this.user_email = history.state.email;
    this.map = new Map({
      target: 'all_device_map',
      layers: [
        new TileLayer({
          source: new OSM()
        })
      ],
      view: new View({
        center: fromLonLat([this.longitude,this.latitude]),
        zoom: this.zoom
      })
    });

    this.service.allDeviceInfo()
      .subscribe(Response => {
        let markers = new Array;
        Response.forEach((device: Device) => {
          if (device.latitude != null && device.longitude != null){
            let device_location = fromLonLat([device.longitude, device.latitude]);
             
            
            var aFeature = new Feature({
              geometry: new Point(device_location)
            })
            var aFeatureStyle = new Style({
              image: new Icon({
                src: '../../assets/images/star.png',
              }),
            })

            aFeature.setStyle(aFeatureStyle);
            aFeature.set('description',this.service.getDeviceDescription(device));
            
            markers.push(aFeature);
          }
        })
        let layer = new Vector({
          source: new VectorSource({features: markers})  
        })
       
        this.map.addLayer(layer);

        let container: HTMLElement | null = document.getElementById('popup');
        let content = document.getElementById('popup-content');
        let closer = document.getElementById('popup-closer');

        

        var popup = new Overlay({
          element: <HTMLInputElement> container,
          autoPan: true,
          autoPanAnimation: {
          duration: 250,
          },
          stopEvent: false,
        });

        this.map.addOverlay(popup);

        // display popup on click
        this.map.on('click',  (evt) => {
          const feature = this.map.forEachFeatureAtPixel(evt.pixel, function (feature: any) {
            return feature;
          });
          if (feature) {
            console.log("got a feature");
            const coordinate = evt.coordinate;
            const hdms = toStringHDMS(toLonLat(coordinate));
        
            container.innerHTML = feature.get('description');
            popup.setPosition(coordinate);
          } else {
            console.log("not a feature");
            popup.setPosition(undefined)
          }
        });
       
      });

      

  }

}
