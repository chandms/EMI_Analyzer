import { Component, OnInit } from '@angular/core';

import * as olsource from 'ol/source';
import Map from 'ol/Map';
import TileLayer from 'ol/layer/Tile';
import View from 'ol/View';
import { fromLonLat } from 'ol/proj';
import Feature from 'ol/Feature';
import { Point } from 'ol/geom';
import { Vector } from 'ol/layer';
import OSM from 'ol/source/OSM';
import Style from 'ol/style/Style';
import Icon from 'ol/style/Icon';

import { DeviceService } from '../service/device.service';
import { Device } from '../device/device.component';
import { User } from '../login/User';
import { Router } from '@angular/router';
import { ActivatedRoute } from '@angular/router';
import { Subscriber } from 'rxjs';


@Component({
  selector: 'map-all',
  templateUrl: './map-all.component.html',
  styleUrls: ['./map-all.component.css']
})
export class MapAllComponent implements OnInit {

  constructor(private service: DeviceService,  private route : ActivatedRoute) { }

  map!: Map;
  zoom: number = 7;
  latitude: number = 39.749926;
  longitude: number = -86.148711;
  user_first_name: string ="";
  user_last_name: string ="";
  user_email = "";


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
                //src: 'https://thumbs.gfycat.com/EvenRemarkableAnole-max-1mb.gif'
                src: '../../assets/images/star.png'
              })
            })
            aFeature.setStyle(aFeatureStyle)
            markers.push(aFeature);
          }
        })
        let layer = new Vector({
          source: new olsource.Vector({features: markers})  
        })
        this.map.addLayer(layer);
      });

  }

}
